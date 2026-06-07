#include "stdafx.hpp"
#include "game/Apple.hpp"

#include "render/RenderContext.hpp"

Apple::Apple()
    : Position{}, TileSize(0.f), bIsSpawned(false)
{
    AppleShape.setFillColor(sf::Color::Red);
    AppleShape.setOutlineColor(sf::Color(170, 0, 0));
}

void Apple::SetTileSize(float tileSize)
{
    TileSize = tileSize;
    AppleShape.setSize(sf::Vector2f(TileSize, TileSize));
    AppleShape.setOutlineThickness(-std::max(2.f, TileSize / 10.f));
}

bool Apple::Spawn(const std::vector<sf::Vector2i>& freeTiles)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, freeTiles.size() - 1);

    if (freeTiles.empty())
    {
        Position = sf::Vector2i(-1, -1);
        bIsSpawned = false;
        return false;
    }

    Position = freeTiles[distr(gen)];
    bIsSpawned = true;
    return true;
}

void Apple::Render(sf::RenderTarget &target, float offsetX, float offsetY)
{
    if (!bIsSpawned)
    {
        return;
    }

    AppleShape.setPosition(
        offsetX + static_cast<float>(Position.x) * TileSize,
        offsetY + static_cast<float>(Position.y) * TileSize
    );
    target.draw(AppleShape);
}

void Apple::FillContext(RenderContext &context, const sf::Vector2f& offset)
{

    AppleShape.setPosition(offset + sf::Vector2f(Position) * TileSize);
    context.Drawables.emplace_back(AppleShape);
}
