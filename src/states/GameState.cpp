#include "stdafx.hpp"
#include "states/GameState.hpp"

#include "states/StateStackManager.hpp"
#include "states/MainMenuState.hpp"

#include "settings/GameSettings.hpp"

#include "game/grid/Grid.hpp"
#include "game/grid/RectangularGrid.hpp"
#include "game/grid/RectangularDonutGrid.hpp"

#include "game/snake/Snake.hpp"
#include "game/snake/BasicSnake.hpp"

#include "utils/IniParser.hpp"
#include "utils/KeyMapping.hpp"

#include "config/Colors.hpp"

GameState::GameState(StateContext& context)
    : IState(context),
    KeyState(0u),
    Grid(CreateGrid()),
    PlayerSnake(CreateSnake()),
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

    UpdateUIScaling();

    InstructionsOverlay.Show();
}

GameState::~GameState() = default;

void GameState::Update(float dt)
{
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

    PlayerSnake->Update(dt);

    if (PlayerSnake->HasFilledGrid())
    {
        EndGameMenu.SetTitle("Snake is full. So is your glory!");
        EndGameMenu.Show();
        return;
    }

    if (!PlayerSnake->IsAlive())
    {
        EndGameMenu.SetTitle("Game Over");
        EndGameMenu.Show();
        return;
    }

    if (Apples.eatAppleAt(PlayerSnake->GetHeadPosition()))
    {
        Apples.spawn(Grid->GetFreeTiles());
        PlayerSnake->Grow(1u);
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

    PlayerSnakeRenderer.render(target, *PlayerSnake, GridOffset);

    if (!PlayerSnake->HasFilledGrid())
    {
        Apples.render(target, GridOffset.x, GridOffset.y);
    }

    target.draw(ScoreText);

    InstructionsOverlay.Render(target);
    PauseMenu.Render(target);
    EndGameMenu.Render(target);
}

void GameState::OnWindowResize(const sf::Event::SizeEvent& size)
{
    UpdateUIScaling();

    const sf::Vector2f windowSize(size.width, size.height);

    InstructionsOverlay.OnWindowResize(windowSize);
    PauseMenu.OnWindowResize(windowSize);
    EndGameMenu.OnWindowResize(windowSize);
}

void GameState::OnKeyPressed(const sf::Event::KeyEvent& key)
{
    if (!Context.Window->hasFocus())
    {
        return;
    }

    if (!InstructionsOverlay.IsActive() && !EndGameMenu.IsActive())
    {
        if (Keybinds.at(EAction::TogglePause) == key.code)
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

    // Closes the InstructionOverlay when any button is pressed
    if (InstructionsOverlay.IsActive())
    {
        InstructionsOverlay.Close();
    }

    static constexpr std::pair<EAction, EMoveDirection> actionToDirection[] {
        { EAction::MoveUp,       EMoveDirection::Up },
        { EAction::MoveDown,     EMoveDirection::Down },
        { EAction::MoveRight,    EMoveDirection::Right },
        { EAction::MoveLeft,     EMoveDirection::Left },
        { EAction::AltMoveUp,    EMoveDirection::Up },
        { EAction::AltMoveDown,  EMoveDirection::Down },
        { EAction::AltMoveRight, EMoveDirection::Right },
        { EAction::AltMoveLeft,  EMoveDirection::Left }
    };

    for (const auto& [action, direction] : actionToDirection)
    {
        if (Keybinds.at(action) == key.code &&
            !KeyState.IsDown(action))
        {
            KeyState.SetDown(action);
            PlayerSnake->ChangeDirection(direction);
            break;
        }
    }
}

void GameState::OnKeyReleased(const sf::Event::KeyEvent& key)
{
    static constexpr EAction actions[] {
        EAction::MoveUp,
        EAction::MoveDown,
        EAction::MoveRight,
        EAction::MoveLeft,
        EAction::AltMoveUp,
        EAction::AltMoveDown,
        EAction::AltMoveRight,
        EAction::AltMoveLeft,
        EAction::TogglePause
    };

    for (const EAction action : actions)
    {
        if (Keybinds.at(action) == key.code)
        {
            KeyState.UnsetDown(action);
            break;
        }
    }
}


void GameState::InitKeybinds()
{
    IniParser iniParser("config/gamestate_keybinds.ini");

    Keybinds.reserve(9);

    Keybinds[EAction::MoveUp]    = KeyMapping::ToKey(iniParser.getString("Snake", "MoveUp",    "W"));
    Keybinds[EAction::MoveDown]  = KeyMapping::ToKey(iniParser.getString("Snake", "MoveDown",  "S"));
    Keybinds[EAction::MoveRight] = KeyMapping::ToKey(iniParser.getString("Snake", "MoveRight", "D"));
    Keybinds[EAction::MoveLeft]  = KeyMapping::ToKey(iniParser.getString("Snake", "MoveLeft",  "A"));

    Keybinds[EAction::AltMoveUp]    = KeyMapping::ToKey(iniParser.getString("Snake", "AltMoveUp",    "Up"));
    Keybinds[EAction::AltMoveDown]  = KeyMapping::ToKey(iniParser.getString("Snake", "AltMoveDown",  "Down"));
    Keybinds[EAction::AltMoveRight] = KeyMapping::ToKey(iniParser.getString("Snake", "AltMoveRight", "Right"));
    Keybinds[EAction::AltMoveLeft]  = KeyMapping::ToKey(iniParser.getString("Snake", "AltMoveLeft",  "Left"));

    Keybinds[EAction::TogglePause] = KeyMapping::ToKey(iniParser.getString("General", "TogglePause", "Escape"));
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

std::shared_ptr<IGrid> GameState::CreateGrid()
{
    const GameSettings& gameSettings = Context.CurrentGameSettings;

    switch (gameSettings.GridType.Value)
    {
        case EGridType::Rectangular:
            return std::make_shared<RectangularGrid>(gameSettings.GridSize.Value);

        case EGridType::RectangularDonut:
            return std::make_shared<RectangularDonutGrid>((gameSettings.GridSize.Value - gameSettings.GridHoleSize.Value) / 2, gameSettings.GridHoleSize.Value);

        default:
            break;
    }

    return std::make_shared<RectangularGrid>(gameSettings.GridSize.Value);
}

std::unique_ptr<ISnake> GameState::CreateSnake()
{
    return std::make_unique<BasicSnake>(Context.CurrentGameSettings.SnakeSpeed.Value, 3u, Grid);
}

void GameState::Restart()
{
    Score = 0u;
    UpdateScoreText();

    Grid = CreateGrid();

    PlayerSnake = CreateSnake();

    Apples.reset();
    Apples.spawnAll(Grid->GetFreeTiles());

    InstructionsOverlay.Show();
    PauseMenu.Close();
    EndGameMenu.Close();
}
