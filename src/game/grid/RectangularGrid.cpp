#include "stdafx.hpp"
#include "game/grid/RectangularGrid.hpp"

RectangularGrid::RectangularGrid(const sf::Vector2i& size)
    : RectangularGrid(size, EInitMode::Full) {}

RectangularGrid::RectangularGrid(const sf::Vector2i& size, EInitMode initMode)
    : Size(size), Occupied(size.x, std::vector<bool>(size.y, false))
{
    assert(size.x >= MinGridSize);
    assert(size.y >= MinGridSize);

    if (initMode == EInitMode::Full)
    {
        InitFreeTiles();
    }
}

bool RectangularGrid::IsWithinBoundaries(const sf::Vector2i& position) const
{
    return position.x >= 0
        && position.y >= 0
        && position.x < Size.x
        && position.y < Size.y;
}

bool RectangularGrid::IsOccupied(const sf::Vector2i& position) const
{
    return Occupied[position.x][position.y];
}

sf::Vector2i RectangularGrid::GetSnakeHeadSpawnPosition() const
{
    const int x = std::max(Size.x / 2 - 1, 0);
    const int y = std::max(Size.y / 2 - 1, 0);

    return sf::Vector2i{x, y};
}

size_t RectangularGrid::GetTotalTileCount() const
{
    return static_cast<size_t>(Size.x * Size.y);
}

bool RectangularGrid::FreeTile(const sf::Vector2i& position)
{
    if (!IsWithinBoundaries(position) || !IsOccupied(position))
    {
        return false;
    }

    FreeTiles.emplace_back(position);
    Occupied[position.x][position.y] = false;
    return true;
}

bool RectangularGrid::OccupyTile(const sf::Vector2i& position)
{
    if (!IsWithinBoundaries(position) || IsOccupied(position))
    {
        return false;
    }

    RemoveFromFreeTiles(position);
    Occupied[position.x][position.y] = true;
    return true;
}

void RectangularGrid::InitFreeTiles()
{
    FreeTiles.reserve(GetTotalTileCount());
    for (int x = 0; x < Size.x; ++x)
    {
        for (int y = 0; y < Size.y; ++y)
        {
            const sf::Vector2i pos{x, y};
            if (IsWithinBoundaries(pos))
            {
                FreeTiles.emplace_back(pos);
            }
        }
    }
}

void RectangularGrid::RemoveFromFreeTiles(const sf::Vector2i &position)
{
    for (auto& tile : FreeTiles)
    {
        if (tile == position)
        {
            tile = FreeTiles.back();
            FreeTiles.pop_back();
            break;
        }
    }
}
