#include "stdafx.hpp"
#include "Game.hpp"

#include "states/GameState.hpp"

#include "utils/IniParser.hpp"

#include "config/Colors.hpp"

#ifdef __linux__
    #include <X11/Xlib.h>
#endif // __linux__

void Game::initWindow()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    IniParser iniParser;
    iniParser.loadFromFile("config/video.ini");

    sf::ContextSettings gfxSetting = sf::ContextSettings();
    gfxSetting.antialiasingLevel = iniParser.getInt("Graphics", "iAntiAliasing", 4);

    unsigned int width = iniParser.getInt("Graphics", "iResolutionWidth");
    unsigned int height = iniParser.getInt("Graphics", "iResolutionHeight");

    if (width == 0) width = desktopMode.width;
    if (height == 0) height = desktopMode.height;

    this->window = new sf::RenderWindow(
        sf::VideoMode({ width, height }),
        "SFML project",
        (iniParser.getBool("Graphics", "bFullscreen", true) ? sf::Style::Fullscreen : sf::Style::Default),
        gfxSetting
    );
    this->window->setFramerateLimit(iniParser.getInt("Graphics", "iFramerateLimit", 60));
    this->window->setVerticalSyncEnabled(iniParser.getBool("Graphics", "bVSync", true));
}

void Game::initSupportedKeys()
{
    IniParser iniParser("config/supportedKeys.ini");
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

void Game::initStateData()
{
    this->stateData.window = this->window;
    this->stateData.supportedKeys = &this->supportedKeys;
    this->stateData.font = &this->font;
}

void Game::initStates()
{
    this->states.push(new GameState(&this->stateData));
}

void Game::updateFPS()
{
    this->fpsCounter.update(this->dt);
    this->fpsText.setString(std::to_string(this->fpsCounter.getFPS()) + " fps");
}

Game::Game()
    : dt(0.f), states{}, supportedKeys{}
{
    this->initWindow();
    this->initSupportedKeys();
    this->initFont();
    this->initStateData();
    this->initStates();

    this->fpsText.setFont(this->font);
    this->fpsText.setPosition(sf::Vector2f(5.f, 5.f));
    this->fpsText.setCharacterSize(16u);
    this->fpsText.setFillColor(sf::Color::Green);
}

Game::~Game()
{
    delete window;

    while (!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
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

            this->states.top()->onWindowResize();
        }
    }
}

void Game::update()
{
#ifdef __linux__
    this->ensureMinimumWindowSize();
#endif // __linux__
    this->updateDeltaTime();
    this->updateSFMLEvent();
    this->updateFPS();

    if (this->states.empty()) {
        this->end();
        return;
    }

    if (this->states.top() == nullptr) {
        assert(false && "Critical error: nullptr on state stack!");
        throw std::runtime_error("Critical error: nullptr found on state stack!");
    }

    this->states.top()->update(this->dt);
}

void Game::render()
{
    this->window->clear(sf::Color(Colors::Hex::Background));

    if (this->states.empty()) {
        this->end();
        return;
    }

    if (this->states.top() == nullptr) {
        assert(false && "Critical error: nullptr on state stack!");
        throw std::runtime_error("Critical error: nullptr found on state stack!");
    }

    this->states.top()->render();

    this->window->draw(this->fpsText);

    this->window->display();
}

void Game::end()
{
    this->window->close();
}

#ifdef __linux__

/*
This function exists as a workaround for a known issue on Linux/X11 platforms
where the window size cannot be programmatically changed while the user is actively
resizing the window by holding its edge. During such interactions, SFML's
sf::RenderWindow::setSize or setView might not behave as expected.

The problem is discussed in more detail on the SFML forum:
https://en.sfml-dev.org/forums/index.php?topic=28956.0

There are two potential solutions:

1. Upgrade to SFML 3.0.x and use `sf::WindowBase::setMinimumSize`, which was introduced
   to address this issue. However, this requires building SFML manually from source,
   which I’m not confident doing.

2. Use a workaround, as implemented here: on each frame, we check if the window has been
   resized below the desired minimum size (320x240). If so, we use Xlib directly to resize
   the window back to the minimum dimensions. This approach waits until the user has
   released the window edge before applying the change, which aligns with how X11 handles
   resize events.

Note: Direct use of Xlib is necessary because SFML's setSize function sometimes fails to
apply the desired size reliably in this edge case.
*/
void Game::ensureMinimumWindowSize()
{
    const auto& windowSize = this->window->getSize();
    if (windowSize.x < 320u || windowSize.y < 240u) {
        ::Window x11Window = this->window->getSystemHandle();

        // Open connection to the X server
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Failed to open X display\n";
            return;
        }

        const auto newWidth = std::max(windowSize.x, 320u);
        const auto newHeight = std::max(windowSize.y, 240u);

        // Resize the window using Xlib
        XResizeWindow(display, x11Window, newWidth, newHeight);

        // Flush the request to the X server
        XFlush(display);

        // Close the connection
        XCloseDisplay(display);

        sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(newWidth), static_cast<float>(newHeight));
        this->window->setView(sf::View(visibleArea));
    }
}

#endif // __linux__
