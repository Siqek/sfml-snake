#include "stdafx.hpp"
#include "Game.hpp"
#include "utils/IniParser.hpp"

void Game::initVariables()
{
    this->dt = 0.f;
}

void Game::initWindow()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    IniParser* iniParser = new IniParser();
    iniParser->loadFromFile("config/video.ini");

    sf::ContextSettings gfxSetting = sf::ContextSettings();
    gfxSetting.antialiasingLevel = iniParser->getInt("Graphics", "iAntiAliasing", 4);

    unsigned int width = iniParser->getInt("Graphics", "iResolutionWidth");
    unsigned int height = iniParser->getInt("Graphics", "iResolutionHeight");

    if (width == 0) width = desktopMode.width;
    if (height == 0) height = desktopMode.height;

    this->window = new sf::RenderWindow(
        sf::VideoMode({ width, height }),
        "SFML project",
        (iniParser->getBool("Graphics", "bFullscreen", true) ? sf::Style::Fullscreen : sf::Style::Default),
        gfxSetting
    );
    this->window->setFramerateLimit(iniParser->getInt("Graphics", "iFramerateLimit", 60));
    this->window->setVerticalSyncEnabled(iniParser->getBool("Graphics", "bVSync", true));

    delete iniParser;
}

void Game::initSupportedKeys()
{
    std::unique_ptr<IniParser> iniParser = std::make_unique<IniParser>("config/supportedKeys.ini");
    auto& keys = iniParser->getSection("Keys");

    this->supportedKeys.rehash(keys.size());

    for (const auto& [key, value] : keys) {
        try {
            int val = std::stoi(value);
            this->supportedKeys[key] = val;
        } catch (const std::exception& e) {
            std::cerr << "Error::Game::initSupportedKeys::" << e.what() << '\n';
        }
    }
}

void Game::initStates()
{
    this->states.push(new GameState(this->window));
}

Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initSupportedKeys();
    this->initStates();
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
    while (this->window->pollEvent(this->sfEvent)) {
        if (this->sfEvent.type == sf::Event::Closed) {
            this->end();
        }
    }
}

void Game::update()
{
    this->updateDeltaTime();
    this->updateSFMLEvent();

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

    this->window->display();
}

void Game::end()
{
    this->window->close();
}
