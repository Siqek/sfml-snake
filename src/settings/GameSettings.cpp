#include "stdafx.hpp"
#include "settings/GameSettings.hpp"

#include "utils/IniParser.hpp"

#include "config/GameSettingsOptions.hpp"

GameSettings::GameSettings()
    : gridSize(0, 0), snakeSpeed(0.f), maxAppleCount(0u) {}

GameSettings::GameSettings(const std::string& filename)
    : gridSize(0, 0), snakeSpeed(0.f), maxAppleCount(0u)
{
    this->loadFromFile(filename);
}

void GameSettings::loadFromFile(const std::string& filename)
{
    IniParser iniParser(filename);

    const auto applyOptionById = [&](const std::string& loadedOptionId, const auto& options, auto& settingValue, auto& settingOptionId) {
        if (loadedOptionId.empty())
            return;

        for (const auto& option : options)
        {
            if (option.id == loadedOptionId)
            {
                settingValue = option.value;
                settingOptionId = option.id;
                break;
            }
        }
    };

    std::string loadedGridSizeOptionId = iniParser.getString("GameSettings", "GridSizeOptionId");
    applyOptionById(loadedGridSizeOptionId, GameSettingsOptions::GridSizeOptions, this->gridSize, this->gridSizeOptionId);

    std::string loadedSnakeSpeedOptionId = iniParser.getString("GameSettings", "SnakeSpeedOptionId");
    applyOptionById(loadedSnakeSpeedOptionId, GameSettingsOptions::SnakeSpeedOptions, this->snakeSpeed, this->snakeSpeedOptionId);

    std::string loadedMaxAppleCountOptionId = iniParser.getString("GameSettings", "MaxAppleCountOptionId");
    applyOptionById(loadedMaxAppleCountOptionId, GameSettingsOptions::MaxAppleCountOptions, this->maxAppleCount, this->maxAppleCountOptionId);
}

void GameSettings::saveToFile(const std::string& filename)
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        file << "[GameSettings]\n";
        file << "GridSizeOptionId=" << this->gridSizeOptionId.value_or("") << "\n";
        file << "SnakeSpeedOptionId=" << this->snakeSpeedOptionId.value_or("") << "\n";
        file << "MaxAppleCountOptionId=" << this->maxAppleCountOptionId.value_or("") << "\n";
    }

    file.close();
}
