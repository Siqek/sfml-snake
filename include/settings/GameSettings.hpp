#ifndef GAMESETTINGS_HPP
#define GAMESETTINGS_HPP

class GameSettings
{
public:
    sf::Vector2<uint8_t> gridSize;
    float snakeSpeed;
    unsigned maxAppleCount;

    std::optional<std::string> gridSizeOptionId;
    std::optional<std::string> snakeSpeedOptionId;
    std::optional<std::string> maxAppleCountOptionId;

    GameSettings();
    GameSettings(const std::string_view& filename);

    void loadFromFile(const std::string_view& filename);
    void saveToFile(const std::string_view& filename);
};

#endif // GAMESETTINGS_HPP
