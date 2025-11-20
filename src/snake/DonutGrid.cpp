#include "stdafx.hpp"
#include "snake/DonutGrid.hpp"

DonutGrid::DonutGrid(const sf::Vector2i& ringSize, const sf::Vector2i& holeSize)
    : Grid(sf::Vector2i(ringSize * 2 + holeSize), Grid::InitMode::Partial), ringSize(ringSize), holeSize(holeSize)
{
    assert(ringSize.x >= MinRingSize);
    assert(ringSize.y >= MinRingSize);

    initFreeTiles();
}

bool DonutGrid::isWithinBoundaries(const sf::Vector2i& position) const
{
    return Grid::isWithinBoundaries(position) && !isWithinHole(position);
}

bool DonutGrid::isWithinHole(const sf::Vector2i& position) const
{
    return position.x >= ringSize.x && position.x < ringSize.x + holeSize.x
        && position.y >= ringSize.y && position.y < ringSize.y + holeSize.y;
}
