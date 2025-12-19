#ifndef APPLECLUSTER_HPP
#define APPLECLUSTER_HPP

#include "game/Apple.hpp"

class AppleCluster
{
public:
    AppleCluster();
    ~AppleCluster() = default;

    void setAppleLimit(unsigned limit) { appleLimit = limit; }
    void setTileSize(float size);

    bool spawn(const std::vector<sf::Vector2i>& freeTiles);
    void spawnAll(const std::vector<sf::Vector2i>& freeTiles);

    bool eatAppleAt(sf::Vector2i snakeHead);

    void reset() { apples.clear(); }

    void render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

private:
    unsigned appleLimit;
    float tileSize;

    std::vector<Apple> apples;
};

#endif // APPLECLUSTER_HPP
