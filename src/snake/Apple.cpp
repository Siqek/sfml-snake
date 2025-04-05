#include "stdafx.hpp"
#include "snake/Apple.hpp"

Apple::Apple()
    : position{}, tileSize(0.f)
{
    this->appleShape.setFillColor(sf::Color::Red);
}

sf::Vector2i Apple::getPosition() const
{
    return this->position;
}

void Apple::setTileSize(float tileSize)
{
    this->tileSize = tileSize;
    this->appleShape.setSize(sf::Vector2f(this->tileSize, this->tileSize));
}

void Apple::spawn(std::vector<sf::Vector2i> freeTiles)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, freeTiles.size() - 1);

    if (freeTiles.empty())
        return;

    this->position = freeTiles[distr(gen)];
}

void Apple::render(sf::RenderTarget &target, float offsetX, float offsetY)
{
    this->appleShape.setPosition(
        offsetX + static_cast<float>(this->position.x) * this->tileSize,
        offsetY + static_cast<float>(this->position.y) * this->tileSize
    );
    target.draw(this->appleShape);
}
