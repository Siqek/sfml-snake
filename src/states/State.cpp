#include "stdafx.hpp"
#include "states/State.hpp"

State::State(sf::RenderWindow* window, const std::unordered_map<std::string, int>& supportedKeys)
    : window(window), supportedKeys(supportedKeys) {}

State::~State() {}
