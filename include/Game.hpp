#ifndef GAME_HPP
#define GAME_HPP

#include "states/State.hpp"

#include "states/StateStackManager.hpp"

#include "settings/GameSettings.hpp"

#include "render/RenderSnapshot.hpp"

#include "utils/FPSCounter.hpp"

class Game
{
public:
    Game();
    ~Game();

    void Run();
    void End();

private:
    void InitWindow();
    void InitFont();
    void InitStates();

    void Update();
    void Render();

    void UpdateLoop();
    void RenderLoop();

    void BuildSnapshot(RenderSnapshot& snapshot);

    void UpdateDeltaTime();
    void UpdateSFMLEvent();
    void UpdateFPS();

    void SetMinimumWindowSize(sf::Vector2i minimumSize);

    void SetMinimumWindowSize_Linux(sf::Vector2i minimumSize);
    void SetMinimumWindowSize_Windows(sf::Vector2i minimumSize);

    std::atomic<bool> bIsRunning;

    std::thread RenderThread;

    RenderSnapshot Snapshots[3];

    RenderSnapshot* ReadSnapshot;
    RenderSnapshot* WriteSnapshot;
    RenderSnapshot* BuiltSnapshot;

    bool bIsNewSnapshotAvailable;

    std::mutex SnapshotMutex;
    std::condition_variable CV;

    sf::RenderWindow* Window;
    std::deque<sf::Event> EventQueue;

    sf::Clock DeltaTimeClock;
    float DeltaTime;

    FPSCounter FpsCounter;
    sf::Text FpsLabel;

    sf::Font AppFont; // is it thread-safe?

    GameSettings Settings;

    StateStackManager StateStack;

    StateContext Context;
};

#endif
