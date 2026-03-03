#ifndef STATE_HPP
#define STATE_HPP

class IState;
class StateStackManager;
class GameSettings;

struct StateContext {
    sf::RenderWindow* Window;
    const sf::Font& AppFont;
    StateStackManager& StateStack;
    GameSettings& CurrentGameSettings;

    StateContext(
        sf::RenderWindow* window,
        const sf::Font& appFont,
        StateStackManager& stateStack,
        GameSettings& currentGameSettings
    )
        : Window(window),
          AppFont(appFont),
          StateStack(stateStack),
          CurrentGameSettings(currentGameSettings)
    {}
};

class IState
{
public:
    virtual ~IState() = default;

    bool WantsToBeDetached() const { return bWantsToBeDetached; }

    virtual void Update(float dt) = 0;

    virtual void Render(sf::RenderTarget& target) = 0;

    virtual void OnWindowResize() {}

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
