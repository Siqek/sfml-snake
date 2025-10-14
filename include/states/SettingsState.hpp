#ifndef SETTINGSSTATE_HPP
#define SETTINGSSTATE_HPP

#include "states/State.hpp"

#include "mgui/ArrowSelector.hpp"

#include "settings/GameSettings.hpp"

class SettingsState
    : public State
{
public:
    SettingsState(StateData* stateData);

    void onWindowResize() override;

    void update(const float& dt) override;
    void render(sf::RenderTarget* target = nullptr) override;
private:
    void updateSelectors();
    void updateButtons();

    void updateUIScaling();

    mgui::ArrowSelector<sf::Vector2<uint8_t>> gridSizeSelector;
    mgui::ArrowSelector<float> snakeSpeedSelector;
    mgui::ArrowSelector<unsigned> maxAppleCountSelector;

    sf::Text gridSizeLabel;
    sf::Text snakeSpeedLabel;
    sf::Text maxAppleCountLabel;

    GameSettings gameSettings;

    mgui::Button saveSettingsButton;
    mgui::Button exitButton;
};

#endif // SETTINGSSTATE_HPP
