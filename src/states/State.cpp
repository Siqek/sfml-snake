#include "stdafx.hpp"
#include "states/State.hpp"

State::State(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font)
    : window(window), supportedKeys(supportedKeys), font(font), mouseScreenPos{}, mouseWindowPos{} {}

State::~State() {}

void State::updateMousePosition()
{
    this->mouseScreenPos = sf::Mouse::getPosition();
    this->mouseWindowPos = sf::Mouse::getPosition(*this->window);
}
