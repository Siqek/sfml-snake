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
    gridSizeSelector(gridSizeOptions, this->font, DefaultGridSizeOptionIndex),
    snakeSpeedSelector(snakeSpeedOptions, this->font, DefaultSnakeSpeedOptionIndex),
    maxAppleCountSelector(maxAppleCountOptions, this->font, DefaultMaxAppleCountOptionIndex)
{
    const auto setElementColors = [](auto& element){
        element.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        element.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        element.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        element.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        element.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        element.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setElementColors(this->gridSizeSelector);
    setElementColors(this->snakeSpeedSelector);
    setElementColors(this->maxAppleCountSelector);

    this->gridSizeLabel.setFont(this->font);
    this->gridSizeLabel.setString("Grid Size:");

    this->snakeSpeedLabel.setFont(this->font);
    this->snakeSpeedLabel.setString("Snake Speed:");

    this->maxAppleCountLabel.setFont(this->font);
    this->maxAppleCountLabel.setString("Max Apple Count:");

    this->saveSettingsButton.setFont(font);
    this->saveSettingsButton.setText("Save");
    setElementColors(this->saveSettingsButton);

    this->exitButton.setFont(this->font);
    this->exitButton.setText("Exit");
    setElementColors(this->exitButton);

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

    this->saveSettingsButton.update(*this->window);
    this->exitButton.update(*this->window);

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

    this->saveSettingsButton.render(*target);
    this->exitButton.render(*target);
}

void SettingsState::updateUIScaling()
{
    const auto windowSize = sf::Vector2f(this->window->getSize());

    const auto selectorSize = sf::Vector2f(windowSize.x / 3.5f, windowSize.y / 18.f);
    const auto firstSelectorPosition = sf::Vector2f(windowSize.x / 3.f * 2.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    const auto selectorPositionOffset = sf::Vector2f(0.f, selectorSize.y * 1.5f);
    const auto selectorOutlineThickness = selectorSize.y / 16.f;

    const auto labelCharacterSize = selectorSize.y / 2.f;
    const auto firstLabelPosition = sf::Vector2f(windowSize.x / 3.f, windowSize.y / 2.f - 1.5f * selectorSize.y);
    const auto labelPositionOffset = selectorPositionOffset;

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

    const auto updateLabelOrigin = [selectorSize](sf::Text& label) {
        auto lb = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(selectorSize.x / 2.f, lb.height / 2.f + lb.top));
    };

    updateLabelOrigin(this->gridSizeLabel);
    updateLabelOrigin(this->snakeSpeedLabel);
    updateLabelOrigin(this->maxAppleCountLabel);

    const auto buttonPositionY = windowSize.y / 8.f * 7.f;
    const auto buttonSize = sf::Vector2f(windowSize.x / 5.f, windowSize.y / 18.f);
    const auto buttonCharacterSize = static_cast<unsigned>(buttonSize.y / 2.f);
    const auto buttonOutlineThickness = buttonSize.y / 20.f;
    const auto buttonMarginFromWindowBorder = windowSize.x / 8.f;

    // Exit button
    this->exitButton.setPosition(sf::Vector2f(buttonMarginFromWindowBorder, buttonPositionY));
    this->exitButton.setSize(buttonSize);
    this->exitButton.setCharacterSize(buttonCharacterSize);
    this->exitButton.setOutlineThickness(buttonOutlineThickness);

    // Save button
    this->saveSettingsButton.setPosition(
        sf::Vector2f(windowSize.x - buttonSize.x - buttonMarginFromWindowBorder, buttonPositionY));
    this->saveSettingsButton.setSize(buttonSize);
    this->saveSettingsButton.setCharacterSize(buttonCharacterSize);
    this->saveSettingsButton.setOutlineThickness(buttonOutlineThickness);
}
