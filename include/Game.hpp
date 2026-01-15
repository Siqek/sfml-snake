#ifndef GAME_HPP
#define GAME_HPP

#include "states/State.hpp"

#include "states/StateStackManager.hpp"

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

    StateStackManager stateStack;

    // TODO(siqek): Use sf::Keyboard::Key enum directly instead of duplicating the structure in a file.
    // Avoid unnecessary overhead and reduce the risk of errors.
    std::unordered_map<std::string, int> supportedKeys;

    StateContext stateContext;

    void initWindow();
    void initSupportedKeys();
    void initFont();
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
