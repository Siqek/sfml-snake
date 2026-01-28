#ifndef RECTANGULARDONUTGRID_HPP
#define RECTANGULARDONUTGRID_HPP

#include "game/grid/RectangularGrid.hpp"

class RectangularDonutGrid
    : public RectangularGrid
{
public:
    RectangularDonutGrid(const sf::Vector2i& ringSize, const sf::Vector2i& holeSize);
    ~RectangularDonutGrid() override = default;

    bool IsWithinBoundaries(const sf::Vector2i& position) const override;

    sf::Vector2i GetSnakeHeadSpawnPosition() const override;

    size_t GetTotalTileCount() const override;

    static constexpr int MinRingSize = 2;

private:
    bool IsWithinHole(const sf::Vector2i& position) const;

    sf::Vector2i RingSize;
    sf::Vector2i HoleSize;
};

#endif // RECTANGULARDONUTGRID_HPP
