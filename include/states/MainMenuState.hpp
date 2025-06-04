#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "states/State.hpp"
#include "mgui/Button.hpp"

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

    mgui::Button startButton;
    mgui::Button exitButton;

    void updateButtons();
    void updateUIScaling();
};

#endif // MAINMENUSTATE_HPP
