#ifndef GAME_HPP
#define GAME_HPP

#include "states/State.hpp"

#include "states/StateStackManager.hpp"

#include "settings/GameSettings.hpp"

#include "render/RenderSnapshot.hpp"

#include "utils/FrameCounter.hpp"

class Game
{
public:
    Game();
    ~Game();

    void Run();

    void RequestEnd();

private:
    void InitWindow();
    void InitFont();
    void InitStates();

    void Update();
    void Render();

    void UpdateLoop();
    void RenderLoop();

    void BuildSnapshot(RenderSnapshot& snapshot);

    void End();

    void UpdateSFMLEvent();

    void SetMinimumWindowSize(sf::Vector2i minimumSize);

    void SetMinimumWindowSize_Linux(sf::Vector2i minimumSize);
    void SetMinimumWindowSize_Windows(sf::Vector2i minimumSize);

    std::atomic<bool> bIsRunning{false};

    std::thread RenderThread;

    sf::RenderWindow* Window;
    std::deque<sf::Event> EventQueue;

    sf::Clock DeltaTimeClock;
    sf::Clock RenderTimeClock;

    FrameCounter FPSCounter; // Frames Per Second (Render)
    FrameCounter UPSCounter; // Updates Per Second (Simulation)

    std::atomic<int> UpdatesPerSecond{0};

    sf::Text FPSLabel;

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
    std::condition_variable NewSnapshotCV;

    bool bIsNewSnapshotAvailable{false}; // shared resource - lock SnapshotMutex to use it safely
};

#endif
