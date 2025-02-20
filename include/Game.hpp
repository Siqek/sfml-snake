#ifndef GAME_HPP
#define GAME_HPP

class State;
class GameState;

#include "states/State.hpp"
#include "states/GameState.hpp"

class Game
{
private:
    sf::RenderWindow* window;
    sf::Clock dtClock;
    float dt;

    sf::Event sfEvent;

    std::stack<State*> states;

    std::unordered_map<std::string, int> supportedKeys;

    void initVariables();
    void initWindow();
    void initSupportedKeys();
    void initStates();

public:
    Game();
    ~Game();

    void run();
    void updateDeltaTime();
    void updateSFMLEvent();
    void update();
    void render();
    void end();
};

#endif
