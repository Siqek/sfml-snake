#include "stdafx.hpp"
#include "states/GameState.hpp"

#include "settings/GameSettings.hpp"

#include "utils/KeyStateTracker.hpp"
#include "utils/IniParser.hpp"

#include "config/Colors.hpp"

void GameState::initKeybinds()
{
    IniParser iniParser("config/gamestate_keybinds.ini");
    const auto& keybindSection = iniParser.getSection("Snake");

    this->keybinds.reserve(keybindSection.size());
    for (const auto& [bind, key] : keybindSection) {
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

void GameState::updateUIScaling()
{
    // Recalculates the size and position of all UI elements based on the new window size
    // such as: tile size, text size/position, and grid offsets

    // new window size
    const sf::Vector2f windowSize(
        static_cast<float>(this->window->getSize().x),
        static_cast<float>(this->window->getSize().y)
    );

    // tile size
    this->tileSize = std::min(
        windowSize.x * 0.95f / static_cast<float>(this->gridSizeX),
        windowSize.y * UIConfig::GridHeightRatio / static_cast<float>(this->gridSizeY)
    );
    this->snake.setTileSize(this->tileSize);
    this->appleCluster.setTileSize(this->tileSize);
    this->tile.setSize(sf::Vector2f(this->tileSize, this->tileSize));

    // score text
    this->scoreText.setCharacterSize(static_cast<unsigned int>(windowSize.y * UIConfig::ScoreHeightRatio * 0.25f));
    this->scoreText.setPosition(sf::Vector2f(
        static_cast<float>(windowSize.x) / 2.f,
        static_cast<float>(windowSize.y) * UIConfig::ScoreHeightRatio / 2.f
    ));

    // centerize score text
    auto lb = this->scoreText.getLocalBounds();
    this->scoreText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    // grid offsets
    this->gridOffsetX = windowSize.x / 2.f - static_cast<float>(this->gridSizeX) / 2.f * this->tileSize;
    this->gridOffsetY =
        windowSize.y * (UIConfig::ScoreHeightRatio + UIConfig::GridHeightRatio / 2.f )
        - static_cast<float>(this->gridSizeY) / 2.f * this->tileSize;

    this->endGameOverlay.onWindowResize(windowSize);
}

GameState::GameState(StateData* stateData)
    : State(stateData),
    gridSizeX(stateData->gameSettings->gridSize.x), gridSizeY(stateData->gameSettings->gridSize.y),
    snake(stateData->gameSettings->snakeSpeed, 3u, this->gridSizeX, this->gridSizeY),
    score(0u),
    endGameOverlay(sf::Vector2f(this->window->getSize()), this->font)
{
    this->updateUIScaling();

    this->appleCluster.setAppleLimit(this->stateData->gameSettings->maxAppleCount);
    this->appleCluster.spawnAll(this->snake.getFreeTiles());

    this->scoreText.setFont(this->font);
    this->scoreText.setString("0");
    this->scoreText.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->scoreText.setOutlineThickness(2.f);
    this->scoreText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    this->initKeybinds();
    this->initKeyStateTracker();
}

GameState::~GameState()
{
    delete this->keyStateTracker;
}

void GameState::onWindowResize()
{
    this->updateUIScaling();
}

void GameState::updateInput()
{
    if (!this->window->hasFocus())
        return;

    this->keyStateTracker->updateKeyStates();

    if (this->keyStateTracker->isKeyDown("MoveUp") || this->keyStateTracker->isKeyDown("AltMoveUp"))
        this->snake.setDirection(Direction::UP);
    else if (this->keyStateTracker->isKeyDown("MoveDown") || this->keyStateTracker->isKeyDown("AltMoveDown"))
        this->snake.setDirection(Direction::DOWN);
    else if (this->keyStateTracker->isKeyDown("MoveRight") || this->keyStateTracker->isKeyDown("AltMoveRight"))
        this->snake.setDirection(Direction::RIGHT);
    else if (this->keyStateTracker->isKeyDown("MoveLeft") || this->keyStateTracker->isKeyDown("AltMoveLeft"))
        this->snake.setDirection(Direction::LEFT);
}

void GameState::update(const float& dt)
{
    this->updateInput();


    if (this->endGameOverlay.getIsActive()) {
        this->endGameOverlay.update(*this->window);

        if (this->endGameOverlay.isBackToMenuButtonReleased())
            this->endState();

        if (this->endGameOverlay.isRestartButtonReleased()) {
            this->endGameOverlay.close();
            this->restart();
        }
    } else {
        this->snake.update(dt);

        if (this->snake.hasFilledGrid()) {
            this->endGameOverlay.setTitle("Snake is full. So is your glory!");
            this->endGameOverlay.show();
        } else if (!this->snake.getIsAlive()) {
            this->endGameOverlay.setTitle("Game Over");
            this->endGameOverlay.show();
        } else if (this->appleCluster.eatAppleAt(this->snake.getHeadPosition()))
        {
            this->appleCluster.spawn(this->snake.getFreeTiles());
            this->snake.grow(1u);
            this->score++;
            this->scoreText.setString(std::to_string(this->score));
        }
    }
}

void GameState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    for (const auto& freeTile : this->snake.getFreeTiles())
    {
        if (freeTile.x % 2 == freeTile.y % 2)
            this->tile.setFillColor(sf::Color(Colors::Hex::BoardCellPrimary));
        else
            this->tile.setFillColor(sf::Color(Colors::Hex::BoardCellSecondary));

        this->tile.setPosition(sf::Vector2f(
            this->gridOffsetX + static_cast<float>(freeTile.x) * this->tileSize,
            this->gridOffsetY + static_cast<float>(freeTile.y) * this->tileSize
        ));
        target->draw(this->tile);
    }

    this->snake.render(*target, this->gridOffsetX, this->gridOffsetY);

    if (!this->snake.hasFilledGrid())
        this->appleCluster.render(*target, this->gridOffsetX, this->gridOffsetY);

    target->draw(this->scoreText);

    this->endGameOverlay.render(*target);
}

void GameState::restart()
{
    this->snake.reset();

    this->appleCluster.reset();
    this->appleCluster.spawnAll(this->snake.getFreeTiles());
}
