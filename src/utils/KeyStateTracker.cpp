#include "stdafx.hpp"
#include "utils/KeyStateTracker.hpp"

void KeyStateTracker::initKeyStates()
{
    for (const auto& [ _, key ] : keys) {
        this->keyStates[key];
    }
}

KeyStateTracker::KeyStateTracker(std::unordered_map<std::string, int> &keys)
    : keys(keys)
{
    this->initKeyStates();
}

void KeyStateTracker::updateKeyStates()
{
    for (auto& [ key, keyState ] : this->keyStates) {
        keyState.previous = keyState.current;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key))) {
            keyState.current = true;
        } else {
            keyState.current = false;
        }
    }
}

bool KeyStateTracker::isKeyPressed(const std::string &key) const
{
    auto it = this->keys.find(key);
    if (it != this->keys.end()) {
        return this->keyStates.at(it->second).current;
    }
    return false;
}

bool KeyStateTracker::isKeyDown(const std::string &key) const
{
    auto it = this->keys.find(key);
    if (it != this->keys.end()) {
        return this->keyStates.at(it->second).current && !this->keyStates.at(it->second).previous;
    }
    return false;
}

bool KeyStateTracker::isKeyUp(const std::string &key) const
{
    auto it = this->keys.find(key);
    if (it != this->keys.end()) {
        return !this->keyStates.at(it->second).current && this->keyStates.at(it->second).previous;
    }
    return false;
}
