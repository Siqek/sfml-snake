#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "states/State.hpp"

#include "mgui/ArrowSelector.hpp"

#include "settings/GameSettings.hpp"

class SettingsState final
    : public IState
{
public:
    SettingsState(StateContext& context);

    void Update(float dt) override;

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

private:
    void UpdateSelectors();
    void UpdateButtons();

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
