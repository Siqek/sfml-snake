#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

class RenderSnapshot;

#include "states/State.hpp"

#include "mgui/ArrowSelector.hpp"

#include "settings/GameSettings.hpp"

class SettingsState final
    : public IState
{
public:
    SettingsState(StateContext& context);

    void Update([[maybe_unused]] float dt) override {};

    void Render(sf::RenderTarget& target) override;

    void BuildSnapshot(RenderSnapshot& snapshot) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

    void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton) override;

    void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton) override;

    void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove) override;

private:
    void SetSelectorCallbacks();

    void SetButtonCallbacks();

    void UpdateUIScaling();

    mgui::ArrowSelector<sf::Vector2i> GridSizeSelector;
    mgui::ArrowSelector<float> SnakeSpeedSelector;
    mgui::ArrowSelector<unsigned> MaxAppleCountSelector;

    sf::Text GridSizeLabel;
    sf::Text SnakeSpeedLabel;
    sf::Text MaxAppleCountLabel;

    GameSettings Settings;

    mgui::Button SaveSettingsButton;
    mgui::Button SaveAndExitButton;
};

#endif // SETTINGSSTATE_HPP
