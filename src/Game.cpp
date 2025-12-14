#include "stdafx.hpp"
#include "Game.hpp"

#include "states/MainMenuState.hpp"

#include "utils/IniParser.hpp"

#include "config/GameSettingsOptions.hpp"

#include "config/Colors.hpp"

#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif // __linux__

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

void Game::initWindow()
{
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    IniParser iniParser;
    iniParser.loadFromFile("config/video.ini");

    sf::ContextSettings gfxSetting = sf::ContextSettings();
    gfxSetting.antialiasingLevel = iniParser.getInt("Graphics", "iAntiAliasing", 4);

    unsigned int width = iniParser.getInt("Graphics", "iResolutionWidth");
    unsigned int height = iniParser.getInt("Graphics", "iResolutionHeight");

    if (width == 0) width = desktopMode.width;
    if (height == 0) height = desktopMode.height;

    bool fullscreen = iniParser.getBool("Graphics", "bFullscreen", true);

    this->window = new sf::RenderWindow(
        sf::VideoMode({ width, height }),
        "SFML project",
        fullscreen ? sf::Style::Fullscreen : sf::Style::Default,
        gfxSetting
    );
    this->window->setFramerateLimit(iniParser.getInt("Graphics", "iFramerateLimit", 60));
    this->window->setVerticalSyncEnabled(iniParser.getBool("Graphics", "bVSync", true));

#if defined(__linux__) || defined(_WIN32)
    this->setMinimumWindowSize(sf::Vector2i(320, 240));
#endif // __linux__ || _WIN32

#ifdef _WIN32
    if (desktopMode.width == this->window->getSize().x
        && desktopMode.height == this->window->getSize().y
        && !fullscreen)
    {
        HWND hwnd = this->window->getSystemHandle();
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
#endif // _WIN32
}

void Game::initSupportedKeys()
{
    IniParser iniParser("config/supported_keys.ini");
    const auto& keys = iniParser.getSection("Keys");

    this->supportedKeys.reserve(keys.size());

    for (const auto& [key, value] : keys) {
        try {
            int val = std::stoi(value);
            this->supportedKeys[key] = val;
        } catch (const std::exception& e) {
            std::cerr << "Error::Game::initSupportedKeys::" << e.what() << '\n';
        }
    }
}

void Game::initFont()
{
    if (!this->font.loadFromFile("resources/fonts/Inter/static/Inter_28pt-Regular.ttf"))
    {
        std::cerr << "Error::Game::initFont::Failed to load font 'resources/fonts/Inter/static/Inter_28pt-Regular.ttf'\n";
    }
}

void Game::initStates()
{
    stateStack.Attach(std::make_shared<MainMenuState>(stateContext));
}

void Game::updateFPS()
{
    this->fpsCounter.update(this->dt);
    this->fpsText.setString(std::to_string(this->fpsCounter.getFPS()) + " fps");
}

Game::Game()
    : dt(0.f), gameSettings("config/game_settings.ini"), supportedKeys{}, stateContext(nullptr, supportedKeys, font, stateStack, gameSettings)
{
    this->initWindow();

    stateContext.Window = window;

    this->initSupportedKeys();
    this->initFont();
    this->initStates();

    this->fpsText.setFont(this->font);
    this->fpsText.setPosition(sf::Vector2f(5.f, 5.f));
    this->fpsText.setCharacterSize(16u);
    this->fpsText.setFillColor(sf::Color::Green);
}

Game::~Game()
{
    delete window;
}

void Game::run()
{
    while (this->window->isOpen())
    {
        this->update();
        this->render();
    }
}

void Game::updateDeltaTime()
{
    this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateSFMLEvent()
{
    sf::Event event;
    while (this->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->end();
        } else if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            this->window->setView(sf::View(visibleArea));

            this->stateStack.OnWindowResize();
        }
    }
}

void Game::update()
{
    this->updateDeltaTime();
    this->updateSFMLEvent();
    this->updateFPS();

    if (stateStack.IsEmpty())
    {
        end();
        return;
    }

    stateStack.UpdateStates(dt);

    stateStack.FlushPendingAttachments();
}

void Game::render()
{
    this->window->clear(sf::Color(Colors::Hex::Background));

    stateStack.RenderStates(*window);

    this->window->draw(this->fpsText);

    this->window->display();
}

void Game::end()
{
    this->window->close();
}

#ifdef __linux__

void Game::setMinimumWindowSize(sf::Vector2i minimumSize)
{
    ::Window x11Window = this->window->getSystemHandle();

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
}

#endif // __linux__

#ifdef _WIN32

void Game::setMinimumWindowSize(sf::Vector2i minimumSize)
{
    Game::MIN_WINDOW_WIDTH = minimumSize.x;
    Game::MIN_WINDOW_HEIGHT = minimumSize.y;

    ::HWND hwnd = this->window->getSystemHandle();

    static WNDPROC oldProc = reinterpret_cast<WNDPROC>(
        GetWindowLongPtr(hwnd, GWLP_WNDPROC)
    );

    auto windowProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (uMsg == WM_GETMINMAXINFO) {
            MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
            mmi->ptMinTrackSize.x = Game::MIN_WINDOW_WIDTH;
            mmi->ptMinTrackSize.y = Game::MIN_WINDOW_HEIGHT;
        }
        return CallWindowProc(oldProc, hwnd, uMsg, wParam, lParam);
    };

    SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(+windowProc));
}

#endif
