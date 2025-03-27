#include "stdafx.hpp"
#include "states/GameState.hpp"
#include "utils/IniParser.hpp"

void GameState::initKeybinds()
{
    IniParser iniParser("config/gamestateKeybinds.ini");
    const auto& keybinds = iniParser.getSection("Snake");

    this->keybinds.reserve(keybinds.size());
    for (const auto& [bind, key] : keybinds) {
        auto it = this->supportedKeys.find(key);
        if (it != this->supportedKeys.end()) {
            this->keybinds[bind] = it->second;
        }
    }
}

void GameState::initKeyStateTracker()
{
    this->keyStateTracker = new KeyStateTracker(this->keybinds);
}

GameState::GameState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font)
    : State(window, supportedKeys, font),
    gridSizeX(12), gridSizeY(12), tileSize(60.f),
    snake(200.f, 3u)
{
    this->snake.setGridSize(this->gridSizeX, this->gridSizeY);
    this->snake.setTileSize(this->tileSize);
    this->snake.initHeadPosition(Position(
        static_cast<int>(this->gridSizeX / 2),
        static_cast<int>(this->gridSizeY / 2)
    ));

    this->tile.setSize(sf::Vector2f(this->tileSize, this->tileSize));
    this->tile.setFillColor(sf::Color(100, 0, 0));
    this->tile.setOutlineThickness(2.f);
    this->tile.setOutlineColor(sf::Color::Magenta);

    this->text.setFont(this->font);
    this->text.setString("Hello World!");
    this->text.setCharacterSize(24);
    this->text.setPosition(sf::Vector2f(1920.f / 2.f, 100.f));
    this->text.setFillColor(sf::Color(255, 255, 255));
    this->text.setOutlineThickness(2.f);
    this->text.setOutlineColor(sf::Color(0, 0, 0));

    // centerize text
    auto lb = this->text.getLocalBounds();
    this->text.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    this->initKeybinds();
    this->initKeyStateTracker();
}

GameState::~GameState()
{
    delete this->keyStateTracker;
}

void GameState::updateInput()
{
    if (!this->window->hasFocus())
        return;

    this->keyStateTracker->updateKeyStates();

    if (this->keyStateTracker->isKeyDown("MoveUp"))
        this->snake.setDirection(Direction::UP);
    else if (this->keyStateTracker->isKeyDown("MoveDown"))
        this->snake.setDirection(Direction::DOWN);
    else if (this->keyStateTracker->isKeyDown("MoveRight"))
        this->snake.setDirection(Direction::RIGHT);
    else if (this->keyStateTracker->isKeyDown("MoveLeft"))
        this->snake.setDirection(Direction::LEFT);
}

void GameState::update(const float& dt)
{
    this->updateInput();

    this->snake.update(dt);
}

void GameState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    float offsetX = static_cast<float>(this->window->getSize().x) / 2.f - static_cast<float>(this->gridSizeX) / 2.f * this->tileSize;
    float offsetY = 200.f;

    for (uint8_t x = 0; x < this->gridSizeX; ++x)
    {
        for (uint8_t y = 0; y < this->gridSizeY; ++y)
        {
            this->tile.setPosition(sf::Vector2f(
                offsetX + static_cast<float>(x) * this->tileSize,
                offsetY + static_cast<float>(y) * this->tileSize
            ));
            target->draw(this->tile);
        }
    }

    this->snake.render(*target, offsetX, offsetY);

    target->draw(this->text);
}
