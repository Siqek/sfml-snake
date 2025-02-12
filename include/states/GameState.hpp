#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class State;

#include "State.hpp"

class GameState
    : public State
{
private:
    sf::CircleShape circle;
public:
    GameState(sf::RenderWindow* window);
    ~GameState();

    void updateInput() override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;
};

#endif
