#include "stdafx.hpp"
#include "snake/AppleCluster.hpp"

AppleCluster::AppleCluster()
    : appleLimit(0u), tileSize(0.f), apples{}
{}

void AppleCluster::setTileSize(float size)
{
    this->tileSize = size;
    for (auto& apple : this->apples)
        apple.setTileSize(size);
}

bool AppleCluster::spawn(const std::vector<sf::Vector2i>& freeTiles)
{
    if (freeTiles.size() <= this->apples.size())
        return false;

    std::vector<sf::Vector2i> trueFreeTiles;
    trueFreeTiles.reserve(freeTiles.size());

    std::vector<sf::Vector2i> applePositions;
    applePositions.reserve(this->apples.size());

    for (const auto& apple : this->apples) {
        applePositions.push_back(apple.getPosition());
    }

    for (const auto& freeTile : freeTiles) {
        bool isFree = true;
        for (auto& applePosition : applePositions) {
            if (freeTile == applePosition) {
                isFree = false;
                applePosition = applePositions.back();
                applePositions.pop_back();
                break;
            }
        }
        if (isFree)
            trueFreeTiles.push_back(freeTile);
    }

    if (trueFreeTiles.empty())
        return false;

    this->apples.emplace_back();
    this->apples.back().setTileSize(this->tileSize);
    return this->apples.back().spawn(trueFreeTiles);
}

void AppleCluster::spawnAll(const std::vector<sf::Vector2i>& freeTiles)
{
    int applesToSpawn = static_cast<int>(this->appleLimit) - static_cast<int>(this->apples.size());
    for (int i = 0; i < applesToSpawn; i++) {
        if (!this->spawn(freeTiles))
            return;
    }
}

bool AppleCluster::eatAppleAt(sf::Vector2i snakeHead)
{
    for (size_t i = 0; i < this->apples.size(); i++) {
        if (this->apples[i].getPosition() == snakeHead) {
            this->apples[i] = this->apples.back();
            this->apples.pop_back();
            return true;
        }
    }
    return false;
}

void AppleCluster::render(sf::RenderTarget& target, float offsetX, float offsetY)
{
    for (size_t i = 0; i < this->apples.size(); i++) {
        this->apples[i].render(target, offsetX, offsetY);
    }
}
