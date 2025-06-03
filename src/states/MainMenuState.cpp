#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

MainMenuState::MainMenuState(StateData* stateData)
    : State(stateData)
{
    rs.setFillColor(sf::Color::Red);
    rs.setSize(sf::Vector2f(20.f, 20.f));

    this->startButtonText.setString("Play");
    this->startButtonText.setFont(this->font);
    this->startButtonText.setFillColor(sf::Color(0x1C1427FF));

    this->startButton.setFillColor(sf::Color(0x40394AFF));
    this->startButton.setOutlineColor(sf::Color(0x1C1427FF));
}

void MainMenuState::onWindowResize()
{
    this->updateUIScaling();
}

void MainMenuState::updateInput()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        if (this->startButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*this->window))))
        {
            // push GameState to state stack (Game::states)
        }
    }
}

void MainMenuState::update(const float& dt)
{
    rs.setPosition(sf::Vector2f(sf::Mouse::getPosition(*this->window)));
}

void MainMenuState::render(sf::RenderTarget* target)
{
    if (target == nullptr)
        target = this->window;

    target->draw(this->startButton);
    target->draw(this->startButtonText);

    target->draw(this->rs);
}

void MainMenuState::updateUIScaling()
{
    sf::Vector2f windowSize(
        static_cast<float>(this->window->getSize().x),
        static_cast<float>(this->window->getSize().y)
    );

    this->startButtonText.setPosition(windowSize / 2.f);
    this->startButtonText.setCharacterSize(static_cast<unsigned>(std::min(windowSize.x * 0.04f, windowSize.y * 0.04f)));

    auto textlb = this->startButtonText.getLocalBounds();
    this->startButtonText.setOrigin(sf::Vector2f(textlb.left + textlb.width / 2.f, textlb.top + textlb.height / 2.f));

    this->startButton.setPosition(this->startButtonText.getPosition());
    this->startButton.setSize(sf::Vector2f(textlb.width * 4.f, textlb.height * 1.8f));

    this->startButton.setOutlineThickness(this->startButton.getSize().y * 0.05f);

    auto buttonlb = this->startButton.getLocalBounds();
    this->startButton.setOrigin(sf::Vector2f(buttonlb.left + buttonlb.width / 2.f, buttonlb.top + buttonlb.height / 2.f));
}
