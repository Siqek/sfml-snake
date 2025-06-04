#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

#include "states/GameState.hpp"

#include "config/Colors.hpp"

MainMenuState::MainMenuState(StateData* stateData)
    : State(stateData)
{
    rs.setFillColor(sf::Color::Red);
    rs.setSize(sf::Vector2f(20.f, 20.f));

    this->startButton.setFont(this->font);
    this->startButton.setText("Play");

    this->exitButton.setFont(this->font);
    this->exitButton.setText("Exit");

    this->startButton.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
    this->startButton.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
    this->startButton.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

    this->startButton.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
    this->startButton.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
    this->startButton.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));

    this->exitButton.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
    this->exitButton.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
    this->exitButton.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

    this->exitButton.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
    this->exitButton.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
    this->exitButton.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
}

void MainMenuState::onWindowResize()
{
    this->updateUIScaling();
}

void MainMenuState::updateInput()
{
}

void MainMenuState::update(const float& /*dt*/)
{
    rs.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this->window)));

    this->updateButtons();
}

void MainMenuState::render(sf::RenderTarget* target)
{
    if (target == nullptr)
        target = this->window;

    this->startButton.render(*target);
    this->exitButton.render(*target);

    target->draw(this->rs);
}

void MainMenuState::updateButtons()
{
    this->startButton.update(*this->window);
    this->exitButton.update(*this->window);

    if (this->startButton.isReleased()) {
        this->stateData->states->push(new GameState(this->stateData));
    }

    if (this->exitButton.isReleased()) {
        this->endState();
    }
}

void MainMenuState::updateUIScaling()
{
    sf::Vector2f windowSize(
        static_cast<float>(this->window->getSize().x),
        static_cast<float>(this->window->getSize().y)
    );

    const unsigned characterSize = static_cast<unsigned>(std::min(windowSize.x / 32.f, windowSize.y / 32.f));
    this->startButton.setCharacterSize(characterSize);
    this->exitButton.setCharacterSize(characterSize);

    const auto tlb = this->startButton.geTextLocalBounds();

    const float buttonWidth = tlb.width * 8.f;
    const float buttonHeight = tlb.height * 1.8f;

    this->startButton.setPosition(windowSize / 2.f);
    this->startButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    this->startButton.setOrigin(this->startButton.getSize() / 2.f);
    this->startButton.setOutlineThickness(buttonHeight / 16.f);

    this->exitButton.setPosition(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f + buttonHeight * 1.8f));
    this->exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    this->exitButton.setOrigin(this->exitButton.getSize() / 2.f);
    this->exitButton.setOutlineThickness(buttonHeight / 16.f);
}
