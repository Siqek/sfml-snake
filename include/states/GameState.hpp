#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

namespace UIConfig {
    static constexpr float ScoreHeightRatio = 0.2f;
    static constexpr float GridHeightRatio = 0.75f;
}

class KeyStateTracker;

#include "states/State.hpp"
#include "snake/Snake.hpp"
#include "snake/AppleCluster.hpp"

#include "states/overlays/EndGameOverlay.hpp"

class GameState
    : public State
{
public:
    GameState(StateData* stateData);
    ~GameState();

    void onWindowResize() override;
    void updateInput() override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;

private:
    std::unordered_map<std::string, int> keybinds;
    KeyStateTracker* keyStateTracker;

    uint8_t gridSizeX;
    uint8_t gridSizeY;
    float gridOffsetX;
    float gridOffsetY;
    float tileSize;
    sf::RectangleShape tile;

    Snake snake;

    AppleCluster appleCluster;

    unsigned int score;
    sf::Text scoreText;

    EndGameOverlay endGameOverlay;

    void initKeybinds();
    void initKeyStateTracker();

    void updateUIScaling();

    void restart();
};

#endif
