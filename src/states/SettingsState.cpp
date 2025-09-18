#include "stdafx.hpp"
#include "states/SettingsState.hpp"

#include "config/Colors.hpp"

const std::vector<mgui::ArrowSelector<sf::Vector2<uint8_t>>::Option> SettingsState::gridSizeOptions = {
    { "8x8", sf::Vector2<uint8_t>(8, 8) },
    { "10x10", sf::Vector2<uint8_t>(10, 10) },
    { "12x12", sf::Vector2<uint8_t>(12, 12) },
};

const std::vector<mgui::ArrowSelector<float>::Option> SettingsState::snakeSpeedOptions = {
    { "slow", 2.f },
    { "normal", 4.f },
    { "fast", 6.f }
};

const std::vector<mgui::ArrowSelector<unsigned>::Option> SettingsState::maxAppleCountOptions = {
    { "1", 1u },
    { "2", 2u },
    { "3", 3u }
};

SettingsState::SettingsState(StateData* stateData)
    : State(stateData),
    gridSizeSelector(gridSizeOptions, this->font),
    snakeSpeedSelector(snakeSpeedOptions, this->font),
    maxAppleCountSelector(maxAppleCountOptions, this->font)
{
    const auto setSelectorColors = [](auto& selector){
        selector.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        selector.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        selector.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        selector.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        selector.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        selector.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setSelectorColors(this->gridSizeSelector);
    setSelectorColors(this->snakeSpeedSelector);
    setSelectorColors(this->maxAppleCountSelector);

    this->updateUIScaling();
}

void SettingsState::onWindowResize()
{
    this->updateUIScaling();
}

void SettingsState::update(const float& /*dt*/)
{
    this->gridSizeSelector.update(*this->window);
    this->snakeSpeedSelector.update(*this->window);
    this->maxAppleCountSelector.update(*this->window);

    if (this->gridSizeSelector.hasActiveOptionChanged())
        this->gameSettings.gridSize = this->gridSizeSelector.getActiveValue();

    if (this->snakeSpeedSelector.hasActiveOptionChanged())
        this->gameSettings.snakeSpeed = this->snakeSpeedSelector.getActiveValue();

    if (this->maxAppleCountSelector.hasActiveOptionChanged())
        this->gameSettings.maxAppleCount = this->maxAppleCountSelector.getActiveValue();
}

void SettingsState::render(sf::RenderTarget* target)
{
    if (target == nullptr)
        target = this->window;

    this->gridSizeSelector.render(*target);
    this->snakeSpeedSelector.render(*target);
    this->maxAppleCountSelector.render(*target);
}

void SettingsState::updateUIScaling()
{
    auto windowSize = sf::Vector2f(this->window->getSize());

    auto selectorSize = sf::Vector2f(windowSize.x / 4.f, windowSize.y / 18.f);

    this->gridSizeSelector.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + 1.5f * selectorSize.y));
    this->gridSizeSelector.setSize(selectorSize);
    this->gridSizeSelector.setOrigin(selectorSize / 2.f);
    this->gridSizeSelector.setOutlineThickness(selectorSize.y / 12.f);

    this->snakeSpeedSelector.setPosition(windowSize / 2.f);
    this->snakeSpeedSelector.setSize(selectorSize);
    this->snakeSpeedSelector.setOrigin(selectorSize / 2.f);
    this->snakeSpeedSelector.setOutlineThickness(selectorSize.y / 12.f);

    this->maxAppleCountSelector.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f - 1.5f * selectorSize.y));
    this->maxAppleCountSelector.setSize(selectorSize);
    this->maxAppleCountSelector.setOrigin(selectorSize / 2.f);
    this->maxAppleCountSelector.setOutlineThickness(selectorSize.y / 12.f);
}
