#include "stdafx.hpp"
#include "settings/GameSettings.hpp"

#include "utils/IniParser.hpp"

#include "config/GameSettingsOptions.hpp"

GameSettings::GameSettings()
    : GridType(GetDefaultGridTypeSetting()),
    GridSize(GetDefaultGridSizeSetting()),
    GridHoleSize(GetDefaultGridHoleSizeSetting()),
    SnakeSpeed(GetDefaultSnakeSpeedSetting()),
    MaxAppleCount(GetDefaultMaxAppleCountSetting())
{}

GameSettings::GameSettings(const std::string& filename)
    : GameSettings()
{
    LoadFromFile(filename);
}

void GameSettings::LoadFromFile(const std::string& filename)
{
    IniParser iniParser(filename);

    const auto findValueById = [](const auto& options, const std::string& id)
        -> const decltype(&options[0].value)
    {
        for (const auto& option : options)
        {
            if (option.id == id)
            {
                return &option.value;
            }
        }
        return nullptr;
    };

    const auto loadSetting = [findValueById](auto& outSetting, const auto& options, const std::string& id, const auto& defaultSetting)
    {
        const auto* value = findValueById(options, id);

        if (value)
        {
            outSetting.Value = *value;
            outSetting.Id = id;
        }
        else
        {
            outSetting = defaultSetting;
        }
    };

    loadSetting(
        GridType,
        GameSettingsOptions::GridTypeOptions,
        iniParser.getString("GameSettings", "GridTypeId"),
        GetDefaultGridTypeSetting()
    );

    loadSetting(
        GridSize,
        GameSettingsOptions::GridSizeOptions,
        iniParser.getString("GameSettings", "GridSizeId"),
        GetDefaultGridSizeSetting()
    );

    loadSetting(
        GridHoleSize,
        GameSettingsOptions::GridHoleSizeOptions,
        iniParser.getString("GameSettings", "GridHoleSizeId"),
        GetDefaultGridHoleSizeSetting()
    );

    loadSetting(SnakeSpeed,
        GameSettingsOptions::SnakeSpeedOptions,
        iniParser.getString("GameSettings", "SnakeSpeedId"),
        GetDefaultSnakeSpeedSetting()
    );

    loadSetting(
        MaxAppleCount,
        GameSettingsOptions::MaxAppleCountOptions,
        iniParser.getString("GameSettings", "MaxAppleCountId"),
        GetDefaultMaxAppleCountSetting()
    );
}

void GameSettings::SaveToFile(const std::string& filename)
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        file << "[GameSettings]\n";
        file << "GridTypeId=" << GridType.Id << "\n";
        file << "GridSizeId=" << GridSize.Id << "\n";
        file << "GridHoleSizeId=" << GridHoleSize.Id << "\n";
        file << "SnakeSpeedId=" << SnakeSpeed.Id << "\n";
        file << "MaxAppleCountId=" << MaxAppleCount.Id << "\n";
    }

    file.close();
}

GameSettings::Setting<EGridType> GameSettings::GetDefaultGridTypeSetting()
{
    const auto& option = GameSettingsOptions::GridTypeOptions[GameSettingsOptions::DefaultGridTypeOptionIndex];
    return Setting<EGridType>(option.id, option.value);
}

GameSettings::Setting<sf::Vector2i> GameSettings::GetDefaultGridSizeSetting()
{
    const auto& option = GameSettingsOptions::GridSizeOptions[GameSettingsOptions::DefaultGridSizeOptionIndex];
    return Setting<sf::Vector2i>(option.id, option.value);
}

GameSettings::Setting<sf::Vector2i> GameSettings::GetDefaultGridHoleSizeSetting()
{
    const auto& option = GameSettingsOptions::GridHoleSizeOptions[GameSettingsOptions::DefaultGridHoleSizeOptionIndex];
    return Setting<sf::Vector2i>(option.id, option.value);
}

GameSettings::Setting<float> GameSettings::GetDefaultSnakeSpeedSetting()
{
    const auto& option = GameSettingsOptions::SnakeSpeedOptions[GameSettingsOptions::DefaultSnakeSpeedOptionIndex];
    return Setting<float>(option.id, option.value);
}

GameSettings::Setting<unsigned> GameSettings::GetDefaultMaxAppleCountSetting()
{
    const auto& option = GameSettingsOptions::MaxAppleCountOptions[GameSettingsOptions::DefaultMaxAppleCountOptionIndex];
    return Setting<unsigned>(option.id, option.value);
}
