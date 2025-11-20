#ifndef DONUTGRID_HPP
#define DONUTGRID_HPP

#include "snake/Grid.hpp"

class DonutGrid
    : public Grid
{
public:
    DonutGrid(const sf::Vector2i& ringSize, const sf::Vector2i& holeSize);
    ~DonutGrid() override = default;

    bool isWithinBoundaries(const sf::Vector2i& position) const override;

    size_t getTotalTileCount() const override { return Grid::getTotalTileCount() - static_cast<size_t>(holeSize.x * holeSize.y); }

private:
    bool isWithinHole(const sf::Vector2i& position) const;

    sf::Vector2i ringSize;
    sf::Vector2i holeSize;

    static constexpr int MinRingSize = std::ceil(MinGridSize / 2);
};

#endif // DONUTGRID_HPP
