#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class State;
class KeyStateTracker;

#include "states/State.hpp"
#include "utils/KeyStateTracker.hpp"

class GameState
    : public State
{
private:
    std::unordered_map<std::string, int> keybinds;
    KeyStateTracker* keyStateTracker;

    uint8_t gridSizeX;
    uint8_t gridSizeY;
    float tileSize;
    sf::RectangleShape tile;

    sf::Text text;

    void initKeybinds();
    void initKeyStateTracker();
public:
    GameState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font);
    ~GameState();

    void updateInput() override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;
};

#endif
