#include "stdafx.hpp"
#include "Game.hpp"
#include "utils/IniParser.hpp"

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

void Game::initStates()
{
    this->states.push(new GameState(this->window, this->supportedKeys, this->font));
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
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            this->window->setView(sf::View(visibleArea));

            this->states.top()->onWindowResize();
        }
    }
}

void Game::update()
{
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
    this->window->clear();

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
