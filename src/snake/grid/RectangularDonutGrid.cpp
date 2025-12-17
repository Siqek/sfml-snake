#include "stdafx.hpp"
#include "snake/grid/RectangularDonutGrid.hpp"

RectangularDonutGrid::RectangularDonutGrid(const sf::Vector2i& ringSize, const sf::Vector2i& holeSize)
    : RectangularGrid(sf::Vector2i(ringSize * 2 + holeSize), RectangularGrid::EInitMode::Partial), RingSize(ringSize), HoleSize(holeSize)
{
    assert(ringSize.x >= MinRingSize);
    assert(ringSize.y >= MinRingSize);

    InitFreeTiles();
}

bool RectangularDonutGrid::IsWithinBoundaries(const sf::Vector2i& position) const
{
    return RectangularGrid::IsWithinBoundaries(position) && !IsWithinHole(position);
}

sf::Vector2i RectangularDonutGrid::GetSnakeHeadSpawnPosition() const
{
    const int x = std::max(GetSize().x / 2 - 1, 0);
    const int y = RingSize.y / 2;

    return sf::Vector2i{x, y};
}

size_t RectangularDonutGrid::GetTotalTileCount() const
{
    return RectangularGrid::GetTotalTileCount() - static_cast<size_t>(HoleSize.x * HoleSize.y);
}

bool RectangularDonutGrid::IsWithinHole(const sf::Vector2i& position) const
{
    return position.x >= RingSize.x && position.x < RingSize.x + HoleSize.x
        && position.y >= RingSize.y && position.y < RingSize.y + HoleSize.y;
}
