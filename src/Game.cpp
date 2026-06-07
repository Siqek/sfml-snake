#include "stdafx.hpp"
#include "Game.hpp"

#include "states/MainMenuState.hpp"

#include "config/GameSettingsOptions.hpp"

#include "config/Colors.hpp"

#include "utils/IniParser.hpp"

#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif // __linux__

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

Game::Game()
    : bIsRunning(false),
      DeltaTime(0.f), Settings("config/game_settings.ini"), Context(sf::Vector2f(0, 0), AppFont, StateStack, Settings),
      Snapshots{ RenderSnapshot(AppFont), RenderSnapshot(AppFont), RenderSnapshot(AppFont) },
      ReadSnapshot(&Snapshots[0]), WriteSnapshot(&Snapshots[1]), BuiltSnapshot(&Snapshots[2]), bIsNewSnapshotAvailable(false)
{
    InitWindow();

    Context.SetWindowSize(sf::Vector2f(Window->getSize()));
    Context.HasWindowFocus.store(Window->hasFocus());

    InitFont();
    InitStates();

    FpsLabel.setFont(AppFont);
    FpsLabel.setPosition(sf::Vector2f(5.f, 5.f));
    FpsLabel.setCharacterSize(16u);
    FpsLabel.setFillColor(sf::Color::Green);
}

Game::~Game()
{
    delete Window;
}

void Game::Run()
{
    bIsRunning = true;

    while (bIsRunning)
    {
        Update();
        Render();
    }

    // TODO(siqek): finish multithreading

    // RenderThread = std::thread(&Game::RenderLoop, this);

    // UpdateLoop();
}

void Game::End()
{
    bIsRunning = false;

    if (RenderThread.joinable())
    {
        RenderThread.join();
    }

    Window->close();
}

void Game::InitWindow()
{
    IniParser iniParser;
    iniParser.loadFromFile("config/video.ini");

    sf::ContextSettings ctxSettings{};
    ctxSettings.antialiasingLevel = iniParser.getInt("Graphics", "iAntiAliasing", 4);

    unsigned int width = iniParser.getInt("Graphics", "iResolutionWidth");
    unsigned int height = iniParser.getInt("Graphics", "iResolutionHeight");

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    if (width == 0) width = desktopMode.width;
    if (height == 0) height = desktopMode.height;

    const bool fullscreen = iniParser.getBool("Graphics", "bFullscreen", true);
    const decltype(sf::Style::Default) windowStyle = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    static constexpr const char* Title = "SFML project";

    Window = new sf::RenderWindow(sf::VideoMode({ width, height }), Title, windowStyle, ctxSettings);

    Window->setFramerateLimit(iniParser.getInt("Graphics", "iFramerateLimit", 60));
    Window->setVerticalSyncEnabled(iniParser.getBool("Graphics", "bVSync", true));

    SetMinimumWindowSize(sf::Vector2i(320, 240));

#ifdef _WIN32
    if (desktopMode.width == Window->getSize().x
        && desktopMode.height == Window->getSize().y
        && !fullscreen)
    {
        HWND hwnd = Window->getSystemHandle();
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
#endif // _WIN32
}

void Game::InitFont()
{
    static constexpr const char* AppFontFileName = "resources/fonts/Inter/static/Inter_28pt-Regular.ttf";
    if (!AppFont.loadFromFile(AppFontFileName))
    {
        throw std::runtime_error(std::string("Failed to load font (") + AppFontFileName + ")");
    }
}

void Game::InitStates()
{
    StateStack.Attach(std::make_shared<MainMenuState>(Context));
}

void Game::Update()
{
    // TODO(siqek):
    UpdateDeltaTime(); // do i need delta time as a member?
    // Window->pollEvent should be called only on main thread so:
    // create temp queue (add events to it from main thread)
    // read the queue from simulation thread
    UpdateSFMLEvent();
    UpdateFPS(); // dont update fps based on simulation

    if (StateStack.IsEmpty())
    {
        End();
        return;
    }

    StateStack.UpdateStates(DeltaTime);

    StateStack.FlushPendingAttachments();
}

void Game::Render()
{
    Window->clear(sf::Color(Colors::Hex::Background));


    {
        // TEMP(siqek): TODO(siqek): building and rendering snapshot test

        // New way of rendering
        // static RenderSnapshot snapshot(AppFont);
        // snapshot.Clear();
        // StateStack.BuildSnapshot(snapshot);
        // snapshot.Render(*Window);

        StateStack.RenderStates(*Window); // Current way of rendering
    }

    Window->draw(FpsLabel);

    Window->display();
}

void Game::UpdateLoop()
{
    // before multithreading, there are few conditions to achieve:

    // TODO(siqek):
    // disallow touching the window in simulation

    // TODO(siqek):
    // update GUI elements (buttons, etc.) based on events
    // don't pass *Window in update methods
    while (bIsRunning)
    {
        Update();

        WriteSnapshot->Clear();
        BuildSnapshot(*WriteSnapshot);

        {
            std::lock_guard<std::mutex> lock(SnapshotMutex);

            std::swap(WriteSnapshot, BuiltSnapshot);

            bIsNewSnapshotAvailable = true;
        }
        CV.notify_one();
    }
}

void Game::RenderLoop()
{
    while (bIsRunning)
    {
        {
            std::unique_lock<std::mutex> lock(SnapshotMutex);

            if (!bIsNewSnapshotAvailable)
            {
                CV.wait(lock, [this]{ return bIsNewSnapshotAvailable; });
            }

            std::swap(ReadSnapshot, BuiltSnapshot);

            bIsNewSnapshotAvailable = false;
        }

        // TODO(siqek):
        // draw everything on sf::RenderTexture and then draw on window?
        // ReadSnapshot->Render(render_target);

        // Window->draw(render_target);
    }
}

void Game::BuildSnapshot(RenderSnapshot &snapshot)
{
    // TODO(siqek): finish it

    (void)snapshot; // ignore *unused args*
}

void Game::UpdateFPS()
{
    FpsCounter.update(DeltaTime);
    FpsLabel.setString(std::to_string(FpsCounter.getFPS()) + " fps");
}

void Game::UpdateDeltaTime()
{
    DeltaTime = DeltaTimeClock.restart().asSeconds();
}

void Game::UpdateSFMLEvent()
{
    sf::Event event;
    while (Window->pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                End();
                break;

            case sf::Event::Resized:
            {
                sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(event.size.width, event.size.height));
                Window->setView(sf::View(visibleArea));

                Context.SetWindowSize(sf::Vector2f(event.size.width, event.size.height));
                StateStack.OnWindowResize(event.size);
                break;
            }

            case sf::Event::LostFocus:
                Context.HasWindowFocus.store(Window->hasFocus());
                break;

            case sf::Event::GainedFocus:
                Context.HasWindowFocus.store(Window->hasFocus());
                break;

            case sf::Event::KeyPressed:
                StateStack.OnKeyPressed(event.key);
                break;

            case sf::Event::KeyReleased:
                StateStack.OnKeyReleased(event.key);
                break;

            case sf::Event::MouseButtonPressed:
                StateStack.OnMouseButtonPressed(event.mouseButton);
                break;

            case sf::Event::MouseButtonReleased:
                StateStack.OnMouseButtonReleased(event.mouseButton);
                break;

            case sf::Event::MouseMoved:
                StateStack.OnMouseMoved(event.mouseMove);
                break;

            default:
                break;
        }
    }
}

void Game::SetMinimumWindowSize(sf::Vector2i minimumSize)
{
#if __linux__
    SetMinimumWindowSize_Linux(minimumSize);
#endif // __linux__
#if _WIN32
    SetMinimumWindowSize_Windows(minimumSize);
#endif // _WIN32
}

void Game::SetMinimumWindowSize_Linux([[maybe_unused]] sf::Vector2i minimumSize)
{
#ifdef __linux__
    ::Window x11Window = Window->getSystemHandle();

    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Failed to open X display\n";
        return;
    }

    XSizeHints* hints = XAllocSizeHints();
    if (!hints) {
        std::cerr << "Failed to allocate XSizeHints\n";
        XCloseDisplay(display);
        return;
    }

    hints->flags = PMinSize;
    hints->min_width = minimumSize.x;
    hints->min_height = minimumSize.y;

    XSetNormalHints(display, x11Window, hints);
    XFlush(display);

    XFree(hints);
    XCloseDisplay(display);
#endif // __linux__
}

void Game::SetMinimumWindowSize_Windows([[maybe_unused]] sf::Vector2i minimumSize)
{
#ifdef _WIN32
    static int MinWindowWidth = 0;
    static int MinWindowHeight = 0;

    MinWindowWidth = minimumSize.x;
    MinWindowHeight = minimumSize.y;

    ::HWND hwnd = Window->getSystemHandle();

    static WNDPROC oldProc = reinterpret_cast<WNDPROC>(
        GetWindowLongPtr(hwnd, GWLP_WNDPROC)
    );

    auto windowProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (uMsg == WM_GETMINMAXINFO) {
            MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
            mmi->ptMinTrackSize.x = MinWindowWidth;
            mmi->ptMinTrackSize.y = MinWindowHeight;
        }
        return CallWindowProc(oldProc, hwnd, uMsg, wParam, lParam);
    };

    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(+windowProc));
#endif // _WIN32
}
