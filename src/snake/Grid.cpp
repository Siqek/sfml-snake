#include "stdafx.hpp"
#include "snake/Grid.hpp"

Grid::Grid(const sf::Vector2i& size)
    : Grid(size, InitMode::Full) {}

Grid::Grid(const sf::Vector2i& size, InitMode initMode)
    : size(size), occupied(size.x, std::vector<bool>(size.y, false))
{
    assert(size.x >= MinGridSize);
    assert(size.y >= MinGridSize);

    if (initMode == InitMode::Full)
        initFreeTiles();
}

bool Grid::isWithinBoundaries(const sf::Vector2i& position) const
{
    return position.x >= 0
        && position.y >= 0
        && position.x < size.x
        && position.y < size.y;
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

void Grid::initFreeTiles()
{
    this->freeTiles.reserve(this->getTotalTileCount());
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            const sf::Vector2i pos{x, y};
            if (isWithinBoundaries(pos)) {
                this->freeTiles.emplace_back(pos);
            }
        }
    }
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
