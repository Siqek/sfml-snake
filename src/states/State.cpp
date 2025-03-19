#include "stdafx.hpp"
#include "states/State.hpp"

State::State(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys, const sf::Font& font)
    : window(window), supportedKeys(supportedKeys), font(font) {}

State::~State() {}
