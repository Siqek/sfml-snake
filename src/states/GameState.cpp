#include "stdafx.hpp"
#include "states/GameState.hpp"

#include "settings/GameSettings.hpp"

#include "snake/Grid.hpp"

#include "utils/KeyStateTracker.hpp"
#include "utils/IniParser.hpp"

#include "config/Colors.hpp"

void GameState::initKeybinds()
{
    IniParser iniParser("config/gamestate_keybinds.ini");
    const auto& keybindSnakeSection = iniParser.getSection("Snake");
    const auto& keybindsGeneralSection = iniParser.getSection("General");

    this->keybinds.reserve(keybindSnakeSection.size() + keybindsGeneralSection.size());

    const auto bindSection = [this](const auto& keybindSection) {
        for (const auto& [bind, key] : keybindSection) {
            auto it = this->supportedKeys.find(key);
            if (it != this->supportedKeys.end()) {
                this->keybinds[bind] = it->second;
            }
        }
    };

    bindSection(keybindSnakeSection);
    bindSection(keybindsGeneralSection);
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
    this->snakeRenderer.setTileSize(this->tileSize);
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
    this->gridOffset.x = windowSize.x / 2.f - static_cast<float>(this->gridSizeX) / 2.f * this->tileSize;
    this->gridOffset.y =
        windowSize.y * (UIConfig::ScoreHeightRatio + UIConfig::GridHeightRatio / 2.f )
        - static_cast<float>(this->gridSizeY) / 2.f * this->tileSize;

    this->gameInstructionsOverlay.OnWindowResize(windowSize);
    this->pauseOverlay.OnWindowResize(windowSize);
    this->endGameOverlay.OnWindowResize(windowSize);
}

GameState::GameState(StateData* stateData)
    : State(stateData),
    grid(new Grid(sf::Vector2i(stateData->gameSettings->gridSize))),
    gridSizeX(stateData->gameSettings->gridSize.x), gridSizeY(stateData->gameSettings->gridSize.y),
    snake(stateData->gameSettings->snakeSpeed, 3u, this->grid),
    score(0u),
    gameInstructionsOverlay(sf::Vector2f(this->window->getSize()), this->font),
    pauseOverlay(sf::Vector2f(this->window->getSize()), this->font),
    endGameOverlay(sf::Vector2f(this->window->getSize()), this->font)
{
    this->appleCluster.setAppleLimit(this->stateData->gameSettings->maxAppleCount);
    this->appleCluster.spawnAll(this->grid->getFreeTiles());

    this->scoreText.setFont(this->font);
    this->scoreText.setString("0");
    this->scoreText.setFillColor(sf::Color(Colors::Hex::TextMain));
    this->scoreText.setOutlineThickness(2.f);
    this->scoreText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));
    this->updateScoreText();

    this->initKeybinds();
    this->initKeyStateTracker();

    this->updateUIScaling();

    this->gameInstructionsOverlay.Show();
}

GameState::~GameState()
{
    delete this->keyStateTracker;
    delete this->grid;
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

    const bool isOtherOverlayActive = gameInstructionsOverlay.GetIsActive() || endGameOverlay.GetIsActive() ;
    if (!isOtherOverlayActive)
    {
        if (this->keyStateTracker->isKeyDown("TogglePause")) {
            if (this->pauseOverlay.GetIsActive())
            this->pauseOverlay.Close();
            else
            this->pauseOverlay.Show();
        }
    }

    if (this->keyStateTracker->isKeyDown("MoveUp") || this->keyStateTracker->isKeyDown("AltMoveUp"))
        this->snake.setDirection(Direction::Up);
    else if (this->keyStateTracker->isKeyDown("MoveDown") || this->keyStateTracker->isKeyDown("AltMoveDown"))
        this->snake.setDirection(Direction::Down);
    else if (this->keyStateTracker->isKeyDown("MoveRight") || this->keyStateTracker->isKeyDown("AltMoveRight"))
        this->snake.setDirection(Direction::Right);
    else if (this->keyStateTracker->isKeyDown("MoveLeft") || this->keyStateTracker->isKeyDown("AltMoveLeft"))
        this->snake.setDirection(Direction::Left);
}

void GameState::update(const float& dt)
{
    this->updateInput();

    if (this->endGameOverlay.GetIsActive()) {
        this->endGameOverlay.Update(*this->window);

        if (this->endGameOverlay.IsButtonReleased("BackToMenu"))
            this->endState();

        if (this->endGameOverlay.IsButtonReleased("Restart"))
            this->restart();

        return;
    }

    if (this->gameInstructionsOverlay.GetIsActive()) {
        // Close the GameInstructionsOverlay if any key is pressed
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key))) {
                this->gameInstructionsOverlay.Close();
                break;
            }
        }
        return;
    }

    if (this->pauseOverlay.GetIsActive()) {
        this->pauseOverlay.Update(*this->window);

        if (this->pauseOverlay.IsButtonReleased("Continue"))
            this->pauseOverlay.Close();

        if (this->pauseOverlay.IsButtonReleased("Restart"))
            this->restart();

        if (this->pauseOverlay.IsButtonReleased("BackToMenu"))
            this->endState();

        return;
    }

    this->snake.update(dt);

    if (this->snake.hasFilledGrid()) {
        this->endGameOverlay.SetTitle("Snake is full. So is your glory!");
        this->endGameOverlay.Show();
        return;
    }

    if (!this->snake.getIsAlive()) {
        this->endGameOverlay.SetTitle("Game Over");
        this->endGameOverlay.Show();
        return;
    }

    if (this->appleCluster.eatAppleAt(this->snake.getHeadPosition()))
    {
        this->appleCluster.spawn(this->grid->getFreeTiles());
        this->snake.grow(1u);
        this->score++;
        this->updateScoreText();
    }
}

void GameState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    for (const auto& freeTile : this->grid->getFreeTiles())
    {
        if (freeTile.x % 2 == freeTile.y % 2)
            this->tile.setFillColor(sf::Color(Colors::Hex::BoardCellPrimary));
        else
            this->tile.setFillColor(sf::Color(Colors::Hex::BoardCellSecondary));

        this->tile.setPosition(this->gridOffset + sf::Vector2f(freeTile) * this->tileSize);
        target->draw(this->tile);
    }

    this->snakeRenderer.render(*target, this->snake, this->gridOffset);

    if (!this->snake.hasFilledGrid())
        this->appleCluster.render(*target, this->gridOffset.x, this->gridOffset.y);

    target->draw(this->scoreText);

    this->gameInstructionsOverlay.Render(*target);
    this->pauseOverlay.Render(*target);
    this->endGameOverlay.Render(*target);
}

void GameState::updateScoreText()
{
    this->scoreText.setString(std::to_string(this->score));
    const auto lb = this->scoreText.getLocalBounds();
    this->scoreText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}

void GameState::restart()
{
    this->score = 0;
    this->updateScoreText();

    delete this->grid;
    this->grid = new Grid(sf::Vector2i(this->stateData->gameSettings->gridSize));

    this->snake.reset();

    this->appleCluster.reset();
    this->appleCluster.spawnAll(this->grid->getFreeTiles());

    this->gameInstructionsOverlay.Show();
    this->pauseOverlay.Close();
    this->endGameOverlay.Close();
}
