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
    GameSettings(const std::string& filename);

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);
};

#endif // GAMESETTINGS_HPP
