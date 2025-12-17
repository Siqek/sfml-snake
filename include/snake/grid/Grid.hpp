#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/System/Vector2.hpp>

#include <vector>

enum class EGridType {
    Rectangular,
    RectangularDonut
};

class IGrid
{
public:
    virtual ~IGrid() = default;

    virtual bool IsWithinBoundaries(const sf::Vector2i& position) const = 0;

    virtual bool IsOccupied(const sf::Vector2i& position) const = 0;

    virtual sf::Vector2i GetSnakeHeadSpawnPosition() const = 0;

    virtual const std::vector<sf::Vector2i>& GetFreeTiles() const = 0;

    virtual size_t GetTotalTileCount() const = 0;

    virtual bool FreeTile(const sf::Vector2i& position) = 0;
    virtual bool OccupyTile(const sf::Vector2i& position) = 0;
};

#endif // GRID_HPP
