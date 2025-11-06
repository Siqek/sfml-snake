#include "stdafx.hpp"
#include "snake/Snake.hpp"

#include "snake/Grid.hpp"

#include "config/Colors.hpp"

Snake::Snake(float speedTilesPerSec, unsigned int length, Grid*& grid)
    : speedTilesPerSec(speedTilesPerSec),
    direction(Direction::Right), prevDirection(Direction::Right), nextDirection(Direction::Right),
    grid(grid),
    lengthToGrow(std::max(1u, length) - 1), /* prevent underflow */
    initialLengthToGrow(lengthToGrow),
    tilesTraveled(0.f),
    body{}, isAlive(true)
{
    assert(length != 0);
    assert(grid != nullptr);

    this->initHeadPosition(this->grid->getSize() / 2 - sf::Vector2i(1, 1));
}

bool Snake::hasFilledGrid() const { return body.size() == grid->getTotalTileCount(); }

void Snake::initHeadPosition(const sf::Vector2i& position)
{
    if (this->body.empty())
        this->addHead(position);
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

void Snake::grow(unsigned int lengthToGrow) { this->lengthToGrow += lengthToGrow; }

void Snake::reset()
{
    direction     = Direction::Right;
    prevDirection = Direction::Right;
    nextDirection = Direction::Right;

    tilesTraveled = 0.f;

    body.clear();

    isAlive = true;

    lengthToGrow = initialLengthToGrow;
    this->initHeadPosition(this->grid->getSize() / 2 - sf::Vector2i(1, 1));
}

void Snake::move()
{
    sf::Vector2i head = this->getHeadPosition();

    switch (this->direction)
    {
    case Direction::Right: head.x++; break;
    case Direction::Left:  head.x--; break;
    case Direction::Up:    head.y--; break;
    case Direction::Down:  head.y++; break;
    }

    // Ensure the snake stays within grid boundaries
    if (!this->grid->isWithinBoundaries(head))
        return this->die();

    // Ensure the snake doesn't collide with itself, except for the tail (since the snake can follow its own tail)
    if (this->isCollidingAt(head) && (!this->isTailCollidingAt(head) || this->lengthToGrow > 0)) {
        this->die();
        return;
    }

    this->prevDirection = this->direction;
    this->direction = this->nextDirection;

    // Snake follows its tail
    if (this->isTailCollidingAt(head)) {
        this->body.push_front(head);
        this->body.pop_back();
        return;
    }

    // Snake occupies a new tile
    this->addHead(head);

    // Snake grows
    if (this->lengthToGrow > 0) {
        this->lengthToGrow--;
        return;
    }

    // Snake moves without growing
    this->removeTail();
}

void Snake::addHead(const sf::Vector2i& head)
{
    if (!this->grid->occupyTile(head))
        throw std::runtime_error("Tile is already taken.");
    this->body.push_front(head);
}

void Snake::removeTail()
{
    if (!this->grid->freeTile(this->body.back()))
        throw std::runtime_error("Tile is already freed.");
    this->body.pop_back();
}

bool Snake::isCollidingAt(const sf::Vector2i& position) const
{
    for (const auto& b : this->body) {
        if (b == position)
            return true;
    }

    return false;
}

void Snake::update(const float& dt)
{
    if (this->hasFilledGrid())
        return;

    if (!this->isAlive)
        return;

    this->tilesTraveled += this->speedTilesPerSec * dt;

    if (this->tilesTraveled > 1.f)
    {
        this->move();
        this->tilesTraveled -= 1.f;
    }
}

Direction Snake::getOppositeDirection(Direction direction) const
{
    switch (direction)
    {
    case Direction::Right: return Direction::Left;
    case Direction::Left:  return Direction::Right;
    case Direction::Down:  return Direction::Up;
    case Direction::Up:    return Direction::Down;
    }
    return direction;
}
