#include "stdafx.hpp"
#include "states/GameState.hpp"

#include "states/StateStackManager.hpp"
#include "states/MainMenuState.hpp"

#include "settings/GameSettings.hpp"

#include "snake/grid/RectangularGrid.hpp"
#include "snake/grid/RectangularDonutGrid.hpp"

#include "utils/KeyStateTracker.hpp"
#include "utils/IniParser.hpp"

#include "config/Colors.hpp"

GameState::GameState(StateContext& context)
    : IState(context),
    Grid(new RectangularGrid(Context.CurrentGameSettings.GridSize.Value)),
    PlayerSnake(Context.CurrentGameSettings.SnakeSpeed.Value, 3u, Grid),
    Score(0u),
    InstructionsOverlay(sf::Vector2f(Context.Window->getSize()), Context.AppFont),
    PauseMenu(sf::Vector2f(Context.Window->getSize()), Context.AppFont),
    EndGameMenu(sf::Vector2f(Context.Window->getSize()), Context.AppFont)
{
    Apples.setAppleLimit(Context.CurrentGameSettings.MaxAppleCount.Value);
    Apples.spawnAll(Grid->GetFreeTiles());

    ScoreText.setFont(Context.AppFont);
    ScoreText.setString("0");
    ScoreText.setFillColor(sf::Color(Colors::Hex::TextMain));
    ScoreText.setOutlineThickness(2.f);
    ScoreText.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));
    UpdateScoreText();

    InitKeybinds();
    InitKeyStateTracker();

    UpdateUIScaling();

    InstructionsOverlay.Show();
}

GameState::~GameState()
{
    delete KeyTracker;
    delete Grid;
}

void GameState::Update(float dt)
{
    UpdateInput();

    if (EndGameMenu.IsActive())
    {
        EndGameMenu.Update(*Context.Window);

        if (EndGameMenu.IsButtonReleased(EndGameOverlay::EButton::BackToMenu))
        {
            Context.StateStack.QueueAttach(std::make_shared<MainMenuState>(Context));
            MarkToBeDetached();
        }

        if (EndGameMenu.IsButtonReleased(EndGameOverlay::EButton::Restart))
        {
            Restart();
        }

        return;
    }

    if (InstructionsOverlay.IsActive())
    {
        // Close the GameInstructionsOverlay if any key is pressed
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
            {
                InstructionsOverlay.Close();
                break;
            }
        }
        return;
    }

    if (PauseMenu.IsActive())
    {
        PauseMenu.Update(*Context.Window);

        if (PauseMenu.IsButtonReleased(PauseOverlay::EButton::Continue))
        {
            PauseMenu.Close();
        }

        if (PauseMenu.IsButtonReleased(PauseOverlay::EButton::Restart))
        {
            Restart();
        }

        if (PauseMenu.IsButtonReleased(PauseOverlay::EButton::BackToMenu))
        {
            Context.StateStack.QueueAttach(std::make_shared<MainMenuState>(Context));
            MarkToBeDetached();
        }

        return;
    }

    PlayerSnake.update(dt);

    if (PlayerSnake.hasFilledGrid())
    {
        EndGameMenu.SetTitle("Snake is full. So is your glory!");
        EndGameMenu.Show();
        return;
    }

    if (!PlayerSnake.getIsAlive())
    {
        EndGameMenu.SetTitle("Game Over");
        EndGameMenu.Show();
        return;
    }

    if (Apples.eatAppleAt(PlayerSnake.getHeadPosition()))
    {
        Apples.spawn(Grid->GetFreeTiles());
        PlayerSnake.grow(1u);
        Score++;
        UpdateScoreText();
    }
}

void GameState::Render(sf::RenderTarget& target)
{
    for (const auto& freeTile : Grid->GetFreeTiles())
    {

        Tile.setFillColor(sf::Color(freeTile.x % 2 == freeTile.y % 2 ? Colors::Hex::BoardCellPrimary : Colors::Hex::BoardCellSecondary));

        Tile.setPosition(GridOffset + sf::Vector2f(freeTile) * TileSize);
        target.draw(Tile);
    }

    PlayerSnakeRenderer.render(target, PlayerSnake, GridOffset);

    if (!PlayerSnake.hasFilledGrid())
    {
        Apples.render(target, GridOffset.x, GridOffset.y);
    }

    target.draw(ScoreText);

    InstructionsOverlay.Render(target);
    PauseMenu.Render(target);
    EndGameMenu.Render(target);
}

void GameState::OnWindowResize()
{
    UpdateUIScaling();

    const sf::Vector2f windowSize(Context.Window->getSize());

    InstructionsOverlay.OnWindowResize(windowSize);
    PauseMenu.OnWindowResize(windowSize);
    EndGameMenu.OnWindowResize(windowSize);
}

void GameState::InitKeybinds()
{
    IniParser iniParser("config/gamestate_keybinds.ini");
    const auto& keybindSnakeSection = iniParser.getSection("Snake");
    const auto& keybindsGeneralSection = iniParser.getSection("General");

    Keybinds.reserve(keybindSnakeSection.size() + keybindsGeneralSection.size());

    const auto bindSection = [this](const auto& keybindSection)
    {
        for (const auto& [bind, key] : keybindSection)
        {
            auto it = this->Context.SupportedKeys.find(key);
            if (it != this->Context.SupportedKeys.end())
            {
                this->Keybinds[bind] = it->second;
            }
        }
    };

    bindSection(keybindSnakeSection);
    bindSection(keybindsGeneralSection);
}

void GameState::InitKeyStateTracker()
{
    KeyTracker = new KeyStateTracker(Keybinds);
}

void GameState::UpdateInput()
{
    if (!Context.Window->hasFocus())
    {
        return;
    }

    KeyTracker->updateKeyStates();

    const bool isOtherOverlayActive = InstructionsOverlay.IsActive() || EndGameMenu.IsActive();
    if (!isOtherOverlayActive)
    {
        if (KeyTracker->isKeyDown("TogglePause"))
        {
            if (PauseMenu.IsActive())
            {
                PauseMenu.Close();
            }
            else
            {
                PauseMenu.Show();
            }
        }
    }

    if (KeyTracker->isKeyDown("MoveUp") || KeyTracker->isKeyDown("AltMoveUp"))
    {
        PlayerSnake.setDirection(Direction::Up);
    }
    else if (KeyTracker->isKeyDown("MoveDown") || KeyTracker->isKeyDown("AltMoveDown"))
    {
        PlayerSnake.setDirection(Direction::Down);
    }
    else if (KeyTracker->isKeyDown("MoveRight") || KeyTracker->isKeyDown("AltMoveRight"))
    {
        PlayerSnake.setDirection(Direction::Right);
    }
    else if (KeyTracker->isKeyDown("MoveLeft") || KeyTracker->isKeyDown("AltMoveLeft"))
    {
        PlayerSnake.setDirection(Direction::Left);
    }
}

void GameState::UpdateScoreText()
{
    ScoreText.setString(std::to_string(Score));
    const auto lb = ScoreText.getLocalBounds();
    ScoreText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));
}

void GameState::UpdateUIScaling()
{
    const sf::Vector2f windowSize(Context.Window->getSize());

    const sf::Vector2i gridSize = Context.CurrentGameSettings.GridSize.Value;

    TileSize = std::min(
        windowSize.x * 0.95f / gridSize.x,
        windowSize.y * UIConfig::GridHeightRatio / gridSize.y
    );
    PlayerSnakeRenderer.setTileSize(TileSize);
    Apples.setTileSize(TileSize);
    Tile.setSize(sf::Vector2f(TileSize, TileSize));

    ScoreText.setCharacterSize(static_cast<unsigned>(windowSize.y * UIConfig::ScoreHeightRatio * 0.25f));
    ScoreText.setPosition(sf::Vector2f(
        windowSize.x / 2.f,
        windowSize.y * UIConfig::ScoreHeightRatio / 2.f
    ));

    auto lb = ScoreText.getLocalBounds();
    ScoreText.setOrigin(sf::Vector2f(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f));

    GridOffset.x = windowSize.x / 2.f - gridSize.x / 2.f * TileSize;
    GridOffset.y = windowSize.y * (UIConfig::ScoreHeightRatio + UIConfig::GridHeightRatio / 2.f) - gridSize.y / 2.f * TileSize;
}

std::unique_ptr<IGrid> GameState::CreateGrid()
{
    const GameSettings& gameSettings = Context.CurrentGameSettings;

    switch (gameSettings.GridType.Value)
    {
        case EGridType::Rectangular:
            return std::make_unique<RectangularGrid>(gameSettings.GridSize.Value);

        case EGridType::RectangularDonut:
            return std::make_unique<RectangularDonutGrid>((gameSettings.GridSize.Value - gameSettings.GridHoleSize.Value) / 2, gameSettings.GridHoleSize.Value);

        default:
            break;
    }

    return std::make_unique<RectangularGrid>(gameSettings.GridSize.Value);
}

void GameState::Restart()
{
    Score = 0u;
    UpdateScoreText();

    delete Grid;
    Grid = new RectangularGrid(Context.CurrentGameSettings.GridSize.Value);

    PlayerSnake.reset();

    Apples.reset();
    Apples.spawnAll(Grid->GetFreeTiles());

    InstructionsOverlay.Show();
    PauseMenu.Close();
    EndGameMenu.Close();
}
