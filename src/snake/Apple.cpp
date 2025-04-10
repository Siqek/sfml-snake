#include "stdafx.hpp"
#include "snake/Apple.hpp"

Apple::Apple()
    : position{}, tileSize(0.f), isSpawned(false)
{
    this->appleShape.setFillColor(sf::Color::Red);
    this->appleShape.setOutlineColor(sf::Color(170, 0, 0));
}

sf::Vector2i Apple::getPosition() const
{
    return this->position;
}

void Apple::setTileSize(float tileSize)
{
    this->tileSize = tileSize;
    this->appleShape.setSize(sf::Vector2f(this->tileSize, this->tileSize));
    this->appleShape.setOutlineThickness(-std::max(2.f, this->tileSize / 10.f));
}

void Apple::spawn(const std::vector<sf::Vector2i>& freeTiles)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, freeTiles.size() - 1);

    if (freeTiles.empty()) {
        this->position = sf::Vector2i(-1, -1);
        this->isSpawned = false;
        return;
    }

    this->position = freeTiles[distr(gen)];
    this->isSpawned = true;
}

void Apple::render(sf::RenderTarget &target, float offsetX, float offsetY)
{
    if (!isSpawned)
        return;

    this->appleShape.setPosition(
        offsetX + static_cast<float>(this->position.x) * this->tileSize,
        offsetY + static_cast<float>(this->position.y) * this->tileSize
    );
    target.draw(this->appleShape);
}
