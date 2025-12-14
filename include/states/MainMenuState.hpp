#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "states/State.hpp"
#include "mgui/Button.hpp"

class MainMenuState
    : public IState
{
public:
    MainMenuState(StateContext& context);

    void Update(float dt) override;

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize() override;

private:
    void UpdateButtons();
    void UpdateUIScaling();

    mgui::Button PlayButton;
    mgui::Button GoToSettingsButton;
    mgui::Button ExitButton;
};

#endif // MAINMENUSTATE_HPP
