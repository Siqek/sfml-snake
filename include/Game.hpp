#ifndef GAME_HPP
#define GAME_HPP

#include "states/State.hpp"

#include "settings/GameSettings.hpp"

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

    GameSettings gameSettings;

    StateData stateData;

    std::stack<State*> states;

    std::unordered_map<std::string, int> supportedKeys;

    void initWindow();
    void initSupportedKeys();
    void initFont();
    void initGameSettings();
    void initStateData();
    void initStates();

    void updateDeltaTime();
    void updateSFMLEvent();
    void updateFPS();

#ifdef _WIN32
    inline static int MIN_WINDOW_WIDTH = 0;
    inline static int MIN_WINDOW_HEIGHT = 0;
#endif // _WIN32

#if defined(__linux__) || defined(_WIN32)
    void setMinimumWindowSize(sf::Vector2i minimumSize);
#endif // __linux__ || _WIN32

};

#endif
