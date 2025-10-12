#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

#include "states/GameState.hpp"
#include "states/SettingsState.hpp"

#include "config/Colors.hpp"

MainMenuState::MainMenuState(StateData* stateData)
    : State(stateData)
{
    this->startButton.setFont(this->font);
    this->startButton.setText("Play");

    this->settingsButton.setFont(this->font);
    this->settingsButton.setText("Settings");

    this->exitButton.setFont(this->font);
    this->exitButton.setText("Exit");

    const auto setButtonColors = [](mgui::Button& button){
        button.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        button.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        button.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        button.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        button.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        button.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    setButtonColors(this->startButton);
    setButtonColors(this->settingsButton);
    setButtonColors(this->exitButton);
}

void MainMenuState::onWindowResize()
{
    this->updateUIScaling();
}

void MainMenuState::update(const float& /*dt*/)
{
    this->updateButtons();
}

void MainMenuState::render(sf::RenderTarget* target)
{
    if (target == nullptr)
        target = this->window;

    this->startButton.render(*target);
    this->settingsButton.render(*target);
    this->exitButton.render(*target);
}

void MainMenuState::updateButtons()
{
    this->startButton.update(*this->window);
    this->settingsButton.update(*this->window);
    this->exitButton.update(*this->window);

    if (this->startButton.isReleased()) {
        this->stateData->states->push(new GameState(this->stateData));
    }

    if (this->settingsButton.isReleased()) {
        this->stateData->states->push(new SettingsState(this->stateData));
    }

    if (this->exitButton.isReleased()) {
        this->endState();
    }
}

void MainMenuState::updateUIScaling()
{
    const auto windowSize = sf::Vector2f(this->window->getSize());

    const std::array<mgui::Button*, 3> buttons{
        &this->startButton,
        &this->settingsButton,
        &this->exitButton
    };

    const auto buttonSize = sf::Vector2f(windowSize.x / 3.5f, windowSize.y / 16.f);
    const auto buttonCharacterSize = static_cast<unsigned>(buttonSize.y / 2.f);
    const auto buttonOutlineThickness = buttonSize.y / 16.f;

    const auto buttonPositionOffset = buttonSize.y * 1.6f;
    const auto firstButtonPosition = sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f - buttonPositionOffset);

    for (size_t i = 0; i < buttons.size(); ++i) {
        auto* button = buttons[i];
        button->setPosition(sf::Vector2f(firstButtonPosition.x, firstButtonPosition.y + buttonPositionOffset * static_cast<float>(i)));
        button->setSize(buttonSize);
        button->setCharacterSize(buttonCharacterSize);
        button->setOrigin(buttonSize / 2.f);
        button->setOutlineThickness(buttonOutlineThickness);
    }
}
