#include "stdafx.hpp"
#include "states/State.hpp"

State::State(StateData* stateData)
    : window(stateData->window), supportedKeys(*stateData->supportedKeys), font(*stateData->font), mouseScreenPos{}, mouseWindowPos{} {}

State::~State() {}

void State::updateMousePosition()
{
    this->mouseScreenPos = sf::Mouse::getPosition();
    this->mouseWindowPos = sf::Mouse::getPosition(*this->window);
}
