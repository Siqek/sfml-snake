#ifndef STATE_HPP
#define STATE_HPP

class State;

struct StateData {
    sf::RenderWindow* window;
    const std::unordered_map<std::string, int>* supportedKeys;
    const sf::Font* font;
    std::stack<State*>* states;
};

class State
{
public:
    State(StateData* stateData);
    virtual ~State();

    virtual void updateInput() = 0;
    virtual void update(const float& dt) = 0;

    virtual void render(sf::RenderTarget* target = nullptr) = 0;

    virtual void onWindowResize() {};

    bool getQuit() const { return quit; }

protected:
    sf::RenderWindow* window;
    const std::unordered_map<std::string, int>& supportedKeys;
    const sf::Font& font;
    StateData* stateData;

    void endState() { quit = true; }

private:
    bool quit;
};

#endif
