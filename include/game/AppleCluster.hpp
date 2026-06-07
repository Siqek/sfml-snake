#ifndef APPLECLUSTER_HPP
#define APPLECLUSTER_HPP

class RenderContext;

#include "game/Apple.hpp"

class AppleCluster
{
public:
    AppleCluster();
    ~AppleCluster() = default;

    void SetAppleLimit(unsigned limit) { AppleLimit = limit; }
    void SetTileSize(float size);

    bool Spawn(const std::vector<sf::Vector2i>& freeTiles);
    void SpawnAll(const std::vector<sf::Vector2i>& freeTiles);

    bool EatAppleAt(sf::Vector2i snakeHead);

    void Reset() { Apples.clear(); }

    void Render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

    void FillContext(RenderContext& context, const sf::Vector2f& offset);

private:
    unsigned AppleLimit;
    float TileSize;

    std::vector<Apple> Apples;
};

#endif // APPLECLUSTER_HPP
