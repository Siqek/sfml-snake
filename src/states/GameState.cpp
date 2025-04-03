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
    gridSizeX(40), gridSizeY(10),
    snake(4.f, 3u)
{
    const auto windowSize = [](const auto& ws) -> sf::Vector2f {
        return sf::Vector2f(static_cast<float>(ws.x), static_cast<float>(ws.y));
    }(this->window->getSize());
    this->tileSize = std::min(
        windowSize.x * 0.95f / static_cast<float>(this->gridSizeX),
        windowSize.y * UIConfig::GridHeightRatio / static_cast<float>(this->gridSizeY)
    );

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

    this->scoreText.setFont(this->font);
    this->scoreText.setString("0");
    const auto convertToFontSize = [](float height) -> int {
        // Converts pixel height to font size (75 px ~= 100 font size)
        // Formula: fontSize = (4/3) * height
        static constexpr float factor = 100.f / 75.f;
        return static_cast<int>(height * factor);
    };
    this->scoreText.setCharacterSize(convertToFontSize(windowSize.y * UIConfig::ScoreHeightRatio * 0.25f));
    this->scoreText.setPosition(sf::Vector2f(
        static_cast<float>(windowSize.x) / 2.f,
        static_cast<float>(windowSize.y) * UIConfig::ScoreHeightRatio / 2.f
    ));
    this->scoreText.setFillColor(sf::Color::Black);
    this->scoreText.setOutlineThickness(2.f);
    this->scoreText.setOutlineColor(sf::Color::White);
    // centerize text
    auto lb = this->scoreText.getLocalBounds();
    this->scoreText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

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

    const auto windowSize = this->window->getSize();
    float offsetX = static_cast<float>(windowSize.x) / 2.f - static_cast<float>(this->gridSizeX) / 2.f * this->tileSize;
    float offsetY =
        static_cast<float>(windowSize.y)
            * (UIConfig::ScoreHeightRatio + UIConfig::GridHeightRatio / 2.f )
        - static_cast<float>(this->gridSizeY) / 2.f
            * this->tileSize;

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

    target->draw(this->scoreText);
}
