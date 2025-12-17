#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

namespace UIConfig {
    static constexpr float ScoreHeightRatio = 0.2f;
    static constexpr float GridHeightRatio = 0.75f;
}

class KeyStateTracker;

class IGrid;

#include "states/State.hpp"

#include "snake/Snake.hpp"
#include "snake/SnakeRenderer.hpp"
#include "snake/AppleCluster.hpp"

#include "states/overlays/GameInstructionsOverlay.hpp"
#include "states/overlays/PauseOverlay.hpp"
#include "states/overlays/EndGameOverlay.hpp"

class GameState
    : public IState
{
public:
    GameState(StateContext& context);
    ~GameState();

    void Update(float dt) override;

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize() override;

private:
    void InitKeybinds();
    void InitKeyStateTracker();

    void UpdateInput();

    void UpdateScoreText();

    void UpdateUIScaling();

    std::shared_ptr<IGrid> CreateGrid();

    void Restart();

    std::unordered_map<std::string, int> Keybinds;
    KeyStateTracker* KeyTracker;

    std::shared_ptr<IGrid> Grid;

    sf::Vector2f GridOffset;
    float TileSize;
    sf::RectangleShape Tile;

    Snake PlayerSnake;
    SnakeRenderer PlayerSnakeRenderer;

    AppleCluster Apples;

    unsigned int Score;
    sf::Text ScoreText;

    GameInstructionsOverlay InstructionsOverlay;
    PauseOverlay PauseMenu;
    EndGameOverlay EndGameMenu;
};

#endif
