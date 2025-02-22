#include "stdafx.hpp"
#include "states/State.hpp"

State::State(sf::RenderWindow* window)
{
    this->window = window;
}

State::~State() {}
