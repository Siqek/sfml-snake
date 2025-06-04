#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

#include "states/GameState.hpp"

MainMenuState::MainMenuState(StateData* stateData)
    : State(stateData)
{
    rs.setFillColor(sf::Color::Red);
    rs.setSize(sf::Vector2f(20.f, 20.f));

    this->startButton.setFont(this->font);
    this->startButton.setText("Play");

    this->startButton.setAccentColor(mgui::ButtonState::Hover, sf::Color::Green);
    this->startButton.setAccentColor(mgui::ButtonState::Active, sf::Color::Red);
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

    target->draw(this->rs);
}

void MainMenuState::updateButtons()
{
    this->startButton.update(*this->window);

    if (this->startButton.isReleased()) {
        this->stateData->states->push(new GameState(this->stateData));
    }
}

void MainMenuState::updateUIScaling()
{
    sf::Vector2f windowSize(
        static_cast<float>(this->window->getSize().x),
        static_cast<float>(this->window->getSize().y)
    );

    this->startButton.setCharacterSize(static_cast<unsigned>(std::min(windowSize.x / 32.f, windowSize.y / 32.f)));
    this->startButton.setPosition(windowSize / 2.f);

    const auto tlb = this->startButton.geTextLocalBounds();

    const float buttonWidth = tlb.width * 8.f;
    const float buttonHeight = tlb.height * 1.8f;

    this->startButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    this->startButton.setOrigin(this->startButton.getSize() / 2.f);
    this->startButton.setOutlineThickness(buttonHeight / 16.f);
}
