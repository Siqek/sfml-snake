#include "stdafx.hpp"
#include "settings/GameSettings.hpp"

GameSettings::GameSettings()
    : gridSize(0, 0), snakeSpeed(0.f), maxAppleCount(0u) {}

GameSettings::GameSettings(const std::string_view& filename)
    : gridSize(0, 0), snakeSpeed(0.f), maxAppleCount(0u)
{
    this->loadFromFile(filename);
}

void GameSettings::loadFromFile(const std::string_view& /*filename*/)
{

}

void GameSettings::saveToFile(const std::string_view& /*filename*/)
{

}
