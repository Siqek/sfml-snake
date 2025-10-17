#include "stdafx.hpp"
#include "snake/Grid.hpp"

Grid::Grid(const sf::Vector2i& size)
    : size(size), occupied(size.x, std::vector<bool>(size.y, false))
{
    assert(size.x >= MinGridSize);
    assert(size.y >= MinGridSize);

    this->freeTiles.reserve(size.x * size.y);
    for (int x = 0; x < static_cast<int>(size.x); ++x)
        for (int y = 0; y < static_cast<int>(size.y); ++y)
            this->freeTiles.emplace_back(x, y);
}

bool Grid::isWithinBoundaries(const sf::Vector2i& position) const
{
    return position.x >= 0
        && position.y >= 0
        && position.x < static_cast<int>(size.x)
        && position.y < static_cast<int>(size.y);
}

bool Grid::freeTile(const sf::Vector2i& position)
{
    if (!this->isWithinBoundaries(position) || !isOccupied(position))
        return false;

    this->freeTiles.emplace_back(position);
    this->occupied[position.x][position.y] = false;
    return true;
}

bool Grid::occupyTile(const sf::Vector2i& position)
{
    if (!this->isWithinBoundaries(position) || isOccupied(position))
        return false;

    this->removeFromFreeTiles(position);
    this->occupied[position.x][position.y] = true;
    return true;
}

void Grid::removeFromFreeTiles(const sf::Vector2i &position)
{
    for (auto& tile : this->freeTiles) {
        if (tile == position) {
            tile = this->freeTiles.back();
            this->freeTiles.pop_back();
            break;
        }
    }
}
