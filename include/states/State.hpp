#ifndef STATE_HPP
#define STATE_HPP

class State
{
private:

protected:
    sf::RenderWindow* window;

public:
    State(sf::RenderWindow* window);
    virtual ~State();

    virtual void updateInput() = 0;
    virtual void update(const float& dt) = 0;

    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

#endif
