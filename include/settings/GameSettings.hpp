#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP

#include "game/grid/Grid.hpp"

class GameSettings
{
public:
    GameSettings();
    GameSettings(const std::string& filename);

    void LoadFromFile(const std::string& filename);
    void SaveToFile(const std::string& filename);

    template<typename T>
    struct Setting {
        std::string Id;
        T Value;

        Setting(const std::string& id, const T& value)
            : Id(id), Value(value) {}
    };

    Setting<EGridType> GridType;
    Setting<sf::Vector2i> GridSize;
    Setting<sf::Vector2i> GridHoleSize;
    Setting<float> SnakeSpeed;
    Setting<unsigned> MaxAppleCount;

private:
    static Setting<EGridType> GetDefaultGridTypeSetting();
    static Setting<sf::Vector2i> GetDefaultGridSizeSetting();
    static Setting<sf::Vector2i> GetDefaultGridHoleSizeSetting();
    static Setting<float> GetDefaultSnakeSpeedSetting();
    static Setting<unsigned> GetDefaultMaxAppleCountSetting();
};

#endif // GAMESETTINGS_HPP
