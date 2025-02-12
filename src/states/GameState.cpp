#include "stdafx.hpp"
#include "states/GameState.hpp"

GameState::GameState(sf::RenderWindow* window)
    : State(window)
{
    this->circle.setRadius(200.f);
    this->circle.setFillColor(sf::Color::Green);
}

GameState::~GameState()
{

}

void GameState::updateInput()
{

}

void GameState::update(const float& dt)
{

}

void GameState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;
    
    target->draw(this->circle);
}
