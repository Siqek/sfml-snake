#ifndef STATE_HPP
#define STATE_HPP

class IState;
class StateStackManager;
class GameSettings;

struct StateContext {
    sf::RenderWindow* Window;
    const std::unordered_map<std::string, int>& SupportedKeys;
    const sf::Font& AppFont;
    StateStackManager& StateStack;
    GameSettings& CurrentGameSettings;

    StateContext(
        sf::RenderWindow* window,
        const std::unordered_map<std::string, int>& supportedKeys,
        const sf::Font& appFont,
        StateStackManager& stateStack,
        GameSettings& currentGameSettings
    )
        : Window(window),
          SupportedKeys(supportedKeys),
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
