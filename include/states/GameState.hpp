#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

namespace UIConfig {
    static constexpr float ScoreHeightRatio = 0.2;
    static constexpr float GridHeightRatio = 0.75;
}

class State;
class KeyStateTracker;
class Snake;
class Apple;

#include "states/State.hpp"
#include "utils/KeyStateTracker.hpp"
#include "snake/Snake.hpp"
#include "snake/Apple.hpp"

class GameState
    : public State
{
public:
    GameState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font);
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

    Apple apple;

    unsigned int score;
    sf::Text scoreText;

    void initKeybinds();
    void initKeyStateTracker();

    void updateUIScaling();

    static int convertToFontSize(float height);
};

#endif
