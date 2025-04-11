#ifndef GAME_HPP
#define GAME_HPP

class State;
class GameState;
class FPSCounter;

#include "states/State.hpp"
#include "states/GameState.hpp"
#include "utils/FPSCounter.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();
    void update();
    void render();
    void end();

private:
    sf::RenderWindow* window;
    sf::Clock dtClock;
    float dt;

    FPSCounter fpsCounter;
    sf::Text fpsText;

    sf::Font font;

    std::stack<State*> states;

    std::unordered_map<std::string, int> supportedKeys;

    void initWindow();
    void initSupportedKeys();
    void initFont();
    void initStates();

    void updateDeltaTime();
    void updateSFMLEvent();
    void updateFPS();
};

#endif
