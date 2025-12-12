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

#include "states/overlays/GridSelectionOverlay.hpp"
#include "states/overlays/GameInstructionsOverlay.hpp"
#include "states/overlays/PauseOverlay.hpp"
#include "states/overlays/EndGameOverlay.hpp"

class GameState
    : public State
{
public:
    GameState(StateData* stateData);
    ~GameState();

    void onWindowResize() override;
    void updateInput();
    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;

private:
    void initKeybinds();
    void initKeyStateTracker();

    std::unique_ptr<IGrid> createGrid();

    void updateUIScaling();

    void updateScoreText();

    void restart();

    std::unordered_map<std::string, int> keybinds;
    KeyStateTracker* keyStateTracker;

    // std::unique_ptr<IGrid> grid;
    IGrid* grid;

    sf::Vector2f gridOffset;
    float tileSize;
    sf::RectangleShape tile;

    Snake snake;
    SnakeRenderer snakeRenderer;

    AppleCluster appleCluster;

    unsigned int score;
    sf::Text scoreText;

    GridSelectionOverlay gridSelectionOverlay;
    GameInstructionsOverlay gameInstructionsOverlay;
    PauseOverlay pauseOverlay;
    EndGameOverlay endGameOverlay;
};

#endif
