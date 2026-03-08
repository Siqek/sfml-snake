#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include "states/State.hpp"
#include "mgui/Button.hpp"

class MainMenuState final
    : public IState
{
public:
    MainMenuState(StateContext& context);

    void Update([[maybe_unused]] float dt) override {}

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

    void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton) override;

    void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton) override;

    void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove) override;

private:
    void SetButtonCallbacks();

    void UpdateUIScaling();

    mgui::Button PlayButton;
    mgui::Button GoToSettingsButton;
    mgui::Button ExitButton;
};

#endif // MAINMENUSTATE_HPP
