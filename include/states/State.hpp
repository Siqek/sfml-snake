#ifndef STATE_HPP
#define STATE_HPP

class State
{
protected:
    sf::RenderWindow* window;
    const std::unordered_map<std::string, int>& supportedKeys;
    const sf::Font& font;
public:
    State(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font);
    virtual ~State();

    virtual void updateInput() = 0;
    virtual void update(const float& dt) = 0;

    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

#endif
