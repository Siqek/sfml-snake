#ifndef GAME_HPP
#define GAME_HPP

class State;
class GameState;

#include "states/State.hpp"
#include "states/GameState.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();
    void updateDeltaTime();
    void updateSFMLEvent();
    void update();
    void render();
    void end();

private:
    sf::RenderWindow* window;
    sf::Clock dtClock;
    float dt;

    sf::Font font;

    std::stack<State*> states;

    std::unordered_map<std::string, int> supportedKeys;

    void initWindow();
    void initSupportedKeys();
    void initFont();
    void initStates();
};

#endif
