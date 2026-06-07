#include "stdafx.hpp"
#include "game/AppleCluster.hpp"

#include "render/RenderContext.hpp"

AppleCluster::AppleCluster()
    : AppleLimit(0u), TileSize(0.f), Apples{}
{}

void AppleCluster::SetTileSize(float size)
{
    TileSize = size;
    for (auto& apple : Apples)
    {
        apple.SetTileSize(size);
    }
}

bool AppleCluster::Spawn(const std::vector<sf::Vector2i>& freeTiles)
{
    if (freeTiles.size() <= Apples.size())
    {
        return false;
    }

    std::vector<sf::Vector2i> trueFreeTiles;
    trueFreeTiles.reserve(freeTiles.size());

    std::vector<sf::Vector2i> applePositions;
    applePositions.reserve(Apples.size());

    for (const auto& apple : Apples)
    {
        applePositions.push_back(apple.GetPosition());
    }

    for (const auto& freeTile : freeTiles)
    {
        bool isFree = true;
        for (auto& applePosition : applePositions)
        {
            if (freeTile == applePosition)
            {
                isFree = false;
                applePosition = applePositions.back();
                applePositions.pop_back();
                break;
            }
        }

        if (isFree)
        {
            trueFreeTiles.push_back(freeTile);
        }
    }

    if (trueFreeTiles.empty())
    {
        return false;
    }

    Apples.emplace_back();
    Apples.back().SetTileSize(TileSize);
    return Apples.back().Spawn(trueFreeTiles);
}

void AppleCluster::SpawnAll(const std::vector<sf::Vector2i>& freeTiles)
{
    int applesToSpawn = static_cast<int>(AppleLimit) - static_cast<int>(Apples.size());
    for (int i = 0; i < applesToSpawn; ++i)
    {
        if (!Spawn(freeTiles))
        {
            return;
        }
    }
}

bool AppleCluster::EatAppleAt(sf::Vector2i snakeHead)
{
    for (size_t i = 0; i < Apples.size(); ++i)
    {
        if (Apples[i].GetPosition() == snakeHead)
        {
            Apples[i] = Apples.back();
            Apples.pop_back();
            return true;
        }
    }
    return false;
}

void AppleCluster::Render(sf::RenderTarget& target, float offsetX, float offsetY)
{
    for (auto& apple : Apples)
    {
        apple.Render(target, offsetX, offsetY);
    }
}

void AppleCluster::FillContext(RenderContext& context, const sf::Vector2f& offset)
{
    for (auto& apple : Apples)
    {
        apple.FillContext(context, offset);
    }
}
