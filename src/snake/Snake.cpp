#include "stdafx.hpp"
#include "snake/Snake.hpp"

Snake::Snake(float speedTilesPerSec, unsigned int length)
    : speedTilesPerSec(speedTilesPerSec),
    direction(Direction::RIGHT), prevDirection(Direction::RIGHT), nextDirection(Direction::RIGHT),
    gridSizeX(0), gridSizeY(0), tileSize(0.f),
    lengthToGrow(std::max(0u, length - 1)), /* prevent underflow */
    distanceTraveled(0.f),
    body{}
{
    this->bodyFragment.setFillColor(sf::Color::Green);
}

void Snake::initHeadPosition(sf::Vector2i position)
{
    if (this->body.empty())
        this->body.push_front(position);
}

std::vector<sf::Vector2i> Snake::getUnoccupiedTiles() const
{
    std::vector<sf::Vector2i> unoccupiedTiles{};
    unoccupiedTiles.reserve(this->gridSizeX * this->gridSizeY - this->body.size());

    std::vector<std::vector<bool>> occupiedTiles(this->gridSizeX, std::vector<bool>(this->gridSizeY, false));

    for (const auto& b : this->body) {
        assert(b.x >= 0 && b.x < this->gridSizeX);
        assert(b.y >= 0 && b.y < this->gridSizeY);
        occupiedTiles[b.x][b.y] = true;
    }

    for (size_t x = 0; x < this->gridSizeX; ++x) {
        for (size_t y = 0; y < this->gridSizeY; ++y) {
            if (!occupiedTiles[x][y])
                unoccupiedTiles.emplace_back(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)));
        }
    }

    return unoccupiedTiles;
}

void Snake::setGridSize(uint8_t x, uint8_t y)
{
    this->gridSizeX = x;
    this->gridSizeY = y;
}

void Snake::setTileSize(float size)
{
    this->tileSize = size;
    this->bodyFragment.setSize(sf::Vector2f(this->tileSize, this->tileSize));
}

void Snake::setDirection(Direction direction)
{
    if (this->prevDirection == this->direction)
    {
        if (direction == this->getOppositeDirection(this->prevDirection))
            return; // ignore move if it's the opposite of the last direction

        this->direction = direction;
        this->nextDirection = direction;
    } else {
        if (direction == this->getOppositeDirection(this->direction))
            return;

        this->nextDirection = direction;
    }
}

void Snake::grow(unsigned int lengthToGrow)
{
    this->lengthToGrow += lengthToGrow;
}

void Snake::move()
{
    sf::Vector2i head = this->getHeadPosition();

    switch (this->direction)
    {
    case Direction::RIGHT:
        head.x++;
        break;
    case Direction::LEFT:
        head.x--;
        break;
    case Direction::UP:
        head.y--;
        break;
    case Direction::DOWN:
        head.y++;
        break;
    }

    // Ensure the snake stays within grid boundaries
    if (head.x >= this->gridSizeX || head.x < 0)
        return;
    if (head.y >= this->gridSizeY || head.y < 0)
        return;

    // Ensure the snake doesn't overlap
    if (this->checkCollision(head))
        return;

    this->prevDirection = this->direction;
    this->direction = this->nextDirection;

    this->body.push_front(head);

    if (this->lengthToGrow > 0)
        this->lengthToGrow--;
    else
        this->body.pop_back();
}

bool Snake::checkCollision(sf::Vector2i position) const
{
    for (auto b : this->body) {
        if (b == position)
            return true;
    }

    return false;
}

bool Snake::checkHeadCollision(sf::Vector2i position) const
{
    return this->getHeadPosition() == position;
}

void Snake::update(const float& dt)
{
    this->distanceTraveled += this->getSpeedPixelsPerSec() * dt;

    if (this->distanceTraveled > this->tileSize)
    {
        this->move();
        this->distanceTraveled -= this->tileSize;
    }
}

void Snake::render(sf::RenderTarget& target, float offsetX, float offsetY)
{
    for (auto b : this->body)
    {
        this->bodyFragment.setPosition(sf::Vector2f(
            offsetX + this->tileSize * b.x,
            offsetY + this->tileSize * b.y
        ));
        target.draw(this->bodyFragment);
    }
}

Direction Snake::getOppositeDirection(Direction direction) const
{
    switch (direction)
    {
    case Direction::RIGHT: return Direction::LEFT;
    case Direction::LEFT:  return Direction::RIGHT;
    case Direction::DOWN:  return Direction::UP;
    case Direction::UP:    return Direction::DOWN;
    }
    return direction;
}
