#include "stdafx.hpp"
#include "states/MainMenuState.hpp"

MainMenuState::MainMenuState(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font)
    : State(window, supportedKeys, font)
{
    rs.setFillColor(sf::Color::Red);
    rs.setSize(sf::Vector2f(20.f, 20.f));
}

void MainMenuState::updateInput()
{

}

void MainMenuState::update(const float& dt)
{
    this->updateMousePosition();

    rs.setPosition(sf::Vector2f(
        static_cast<float>(this->mouseWindowPos.x),
        static_cast<float>(this->mouseWindowPos.y)
    ));
}

void MainMenuState::render(sf::RenderTarget* target)
{
    if (target == nullptr)
        target = this->window;

    target->draw(this->rs);
}
