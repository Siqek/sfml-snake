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

    this->gridSizeLabel.setFont(this->font);
    this->gridSizeLabel.setString("Grid Size:");

    this->snakeSpeedLabel.setFont(this->font);
    this->snakeSpeedLabel.setString("Snake Speed:");

    this->maxAppleCountLabel.setFont(this->font);
    this->maxAppleCountLabel.setString("Max Apple Count:");

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

    target->draw(this->gridSizeLabel);
    target->draw(this->snakeSpeedLabel);
    target->draw(this->maxAppleCountLabel);

    this->gridSizeSelector.render(*target);
    this->snakeSpeedSelector.render(*target);
    this->maxAppleCountSelector.render(*target);
}

void SettingsState::updateUIScaling()
{
    auto windowSize = sf::Vector2f(this->window->getSize());

    auto selectorSize = sf::Vector2f(windowSize.x / 3.5f, windowSize.y / 18.f);
    auto firstSelectorPosition = sf::Vector2f(windowSize.x / 3.f * 2.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    auto selectorPositionOffset = sf::Vector2f(0.f, selectorSize.y * 1.5f);
    auto selectorOutlineThickness = selectorSize.y / 16.f;

    auto labelCharacterSize = selectorSize.y / 2.f;
    auto firstLabelPosition = sf::Vector2f(windowSize.x / 3.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    auto labelPositionOffset = selectorPositionOffset;

    // Update selectors
    this->gridSizeSelector.setPosition(firstSelectorPosition);
    this->gridSizeSelector.setSize(selectorSize);
    this->gridSizeSelector.setOrigin(selectorSize / 2.f);
    this->gridSizeSelector.setOutlineThickness(selectorOutlineThickness);

    this->snakeSpeedSelector.setPosition(firstSelectorPosition + selectorPositionOffset);
    this->snakeSpeedSelector.setSize(selectorSize);
    this->snakeSpeedSelector.setOrigin(selectorSize / 2.f);
    this->snakeSpeedSelector.setOutlineThickness(selectorOutlineThickness);

    this->maxAppleCountSelector.setPosition(firstSelectorPosition + 2.f * selectorPositionOffset);
    this->maxAppleCountSelector.setSize(selectorSize);
    this->maxAppleCountSelector.setOrigin(selectorSize / 2.f);
    this->maxAppleCountSelector.setOutlineThickness(selectorOutlineThickness);

    // Update labels
    this->gridSizeLabel.setCharacterSize(labelCharacterSize);
    this->gridSizeLabel.setPosition(firstLabelPosition);

    this->snakeSpeedLabel.setCharacterSize(labelCharacterSize);
    this->snakeSpeedLabel.setPosition(firstLabelPosition + labelPositionOffset);

    this->maxAppleCountLabel.setCharacterSize(labelCharacterSize);
    this->maxAppleCountLabel.setPosition(firstLabelPosition + 2.f * labelPositionOffset);

    float maxLabelWidth = std::max({
        this->gridSizeLabel.getLocalBounds().width,
        this->snakeSpeedLabel.getLocalBounds().width,
        this->maxAppleCountLabel.getLocalBounds().width
    });

    const auto updateLabelOrigin = [maxLabelWidth](sf::Text& label) {
        auto lb = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(maxLabelWidth / 2.f, lb.height / 2.f + lb.top));
    };

    updateLabelOrigin(this->gridSizeLabel);
    updateLabelOrigin(this->snakeSpeedLabel);
    updateLabelOrigin(this->maxAppleCountLabel);
}
