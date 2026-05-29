#ifndef STATE_HPP
#define STATE_HPP

class IState;
class StateStackManager;
class GameSettings;

struct StateContext
{
public:
    StateContext(
        sf::Vector2f windowSize,
        const sf::Font& appFont,
        StateStackManager& stateStack,
        GameSettings& currentGameSettings
    )
        : AppFont(appFont),
          StateStack(stateStack),
          CurrentGameSettings(currentGameSettings),
          WindowSize(windowSize)
    {}

    void SetWindowSize(sf::Vector2f windowSize)
    {
        std::lock_guard<std::mutex> lock(WindowSizeMutex);
        WindowSize = windowSize;
    }

    sf::Vector2f GetWindowSize()
    {
        std::lock_guard<std::mutex> lock(WindowSizeMutex);
        return WindowSize;
    }

    const sf::Font& AppFont;
    StateStackManager& StateStack;
    GameSettings& CurrentGameSettings;
    std::atomic<bool> HasWindowFocus{false};

private:
    sf::Vector2f WindowSize;
    std::mutex WindowSizeMutex;
};

class IState
{
public:
    virtual ~IState() = default;

    bool WantsToBeDetached() const { return bWantsToBeDetached; }

    virtual void Update(float dt) = 0;

    virtual void Render(sf::RenderTarget& target) = 0;

    virtual void OnWindowResize([[maybe_unused]] const sf::Event::SizeEvent& size) {}

    virtual void OnKeyPressed([[maybe_unused]] const sf::Event::KeyEvent& key) {}

    virtual void OnKeyReleased([[maybe_unused]] const sf::Event::KeyEvent& key) {}

    virtual void OnMouseButtonPressed([[maybe_unused]] const sf::Event::MouseButtonEvent& mouseButton) {}

    virtual void OnMouseButtonReleased([[maybe_unused]] const sf::Event::MouseButtonEvent& mouseButton) {}

    virtual void OnMouseMoved([[maybe_unused]] const sf::Event::MouseMoveEvent& mouseMove) {}

    virtual void OnAttach() {}

    virtual void OnDetach() {}

protected:
    IState(StateContext& context);

    void MarkToBeDetached();

    StateContext& Context;

private:
    bool bWantsToBeDetached;
};

#endif
