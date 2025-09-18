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
    void updateUIScaling();

    static const std::vector<mgui::ArrowSelector<sf::Vector2<uint8_t>>::Option> gridSizeOptions;
    mgui::ArrowSelector<sf::Vector2<uint8_t>> gridSizeSelector;

    static const std::vector<mgui::ArrowSelector<float>::Option> snakeSpeedOptions;
    mgui::ArrowSelector<float> snakeSpeedSelector;

    static const std::vector<mgui::ArrowSelector<unsigned>::Option> maxAppleCountOptions;
    mgui::ArrowSelector<unsigned> maxAppleCountSelector;

    GameSettings gameSettings;
};

#endif // SETTINGSSTATE_HPP
