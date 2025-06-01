#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "states/State.hpp"

class MainMenuState
    : public State
{
public:
    MainMenuState(StateData* stateData);

    void onWindowResize() override;

    void updateInput() override;
    void update(const float& dt) override;

    void render(sf::RenderTarget* target = nullptr) override;

private:
    sf::RectangleShape rs;

    sf::RectangleShape startButton;
    sf::Text startButtonText;

    void updateUIScaling();
};

#endif // MAINMENUSTATE_HPP
