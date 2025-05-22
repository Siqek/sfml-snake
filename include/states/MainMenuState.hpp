#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

class State;

#include "states/State.hpp"

class MainMenuState
    : public State
{
public:
    MainMenuState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font);

    void updateInput() override;
    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;
private:
    sf::RectangleShape rs;
};

#endif // MAINMENUSTATE_HPP
