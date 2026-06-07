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

    // snapshot-related variables
    RenderSnapshot Snapshots[3]; // shared resource - lock SnapshotMutex to use it safely

    RenderSnapshot* ReadSnapshot; // shared resource - lock SnapshotMutex to use it safely
    RenderSnapshot* WriteSnapshot; // shared resource - lock SnapshotMutex to use it safely
    RenderSnapshot* BuiltSnapshot; // shared resource - lock SnapshotMutex to use it safely

    std::mutex SnapshotMutex;
    std::condition_variable CV;

    bool bIsNewSnapshotAvailable; // shared resource - lock SnapshotMutex to use it safely
};

#endif
