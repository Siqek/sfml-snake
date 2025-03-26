#include "stdafx.hpp"
#include "snake/Snake.hpp"

Snake::Snake(float speed, unsigned int length)
    : speed(speed), direction(Direction::RIGHT)
{
    this->lengthToGrow = length - 1;
}

Position Snake::getHeadPosition() const
{
    return this->body.front();
}

void Snake::setGridSize(uint8_t x, uint8_t y)
{
    this->gridSizeX = x;
    this->gridSizeY = y;
}

void Snake::setTileSize(float size)
{
    this->tileSize = size;
}

void Snake::setDirection(Direction direction)
{
    this->direction = direction;
}

void Snake::grow(unsigned int length)
{
    this->lengthToGrow += length;
}

void Snake::move()
{
    Position head = this->getHeadPosition();

    switch (this->direction)
    {
    case Direction::RIGHT:
        head.first++;
        break;
    case Direction::LEFT:
        head.first--;
        break;
    case Direction::UP:
        head.second--;
        break;
    case Direction::DOWN:
        head.second++;
        break;
    }

    this->body.push_back(head);

    if (this->lengthToGrow > 0)
        this->lengthToGrow--;
    else
        this->body.pop_back();
}

bool Snake::checkCollision(Position position)
{
    return false;
}

void Snake::update(const float& dt)
{
    this->distanceTraveled += this->speed * dt;

    if (this->distanceTraveled > this->tileSize)
    {
        this->move();
        this->distanceTraveled -= this->tileSize;
    }
}

void Snake::render(sf::RenderTarget& target)
{

}
