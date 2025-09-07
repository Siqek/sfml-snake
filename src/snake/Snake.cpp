#include "stdafx.hpp"
#include "snake/Snake.hpp"

#include "config/Colors.hpp"

Snake::Snake(float speedTilesPerSec, unsigned int length)
    : speedTilesPerSec(speedTilesPerSec),
    direction(Direction::RIGHT), prevDirection(Direction::RIGHT), nextDirection(Direction::RIGHT),
    gridSizeX(0), gridSizeY(0), tileSize(0.f),
    lengthToGrow(std::max(0u, length - 1)), /* prevent underflow */
    initialLengthToGrow(lengthToGrow),
    distanceTraveled(0.f),
    body{}, freeTiles{}, isAlive(true)
{
    this->bodySegment.setFillColor(sf::Color(Colors::Hex::SnakeBody));
    this->bodyBorder.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
    this->bodyBorderCorner.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
}

void Snake::initHeadPosition(const sf::Vector2i& position)
{
    if (this->body.empty()) {
        this->body.push_front(position);
        this->removeFromFreeTiles(position);
    }
}

void Snake::setGridSize(uint8_t x, uint8_t y)
{
    this->gridSizeX = x;
    this->gridSizeY = y;

    this->resetFreeTiles();
}

void Snake::setTileSize(float size)
{
    this->tileSize = size;
    this->bodySegment.setSize(sf::Vector2f(this->tileSize, this->tileSize));

    const auto borderThickness = std::max(2.f, this->tileSize / 10.f);

    this->bodyBorder.setSize(sf::Vector2f(this->tileSize, borderThickness));
    this->bodyBorder.setOrigin(this->bodySegment.getSize() / 2.f);

    this->bodyBorderCorner.setSize(sf::Vector2f(borderThickness, borderThickness));
    this->bodyBorderCorner.setOrigin(this->bodySegment.getSize() / 2.f);
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

void Snake::reset()
{
    direction     = Direction::RIGHT;
    prevDirection = Direction::RIGHT;
    nextDirection = Direction::RIGHT;

    distanceTraveled = 0.f;

    body.clear();
    resetFreeTiles();

    isAlive = true;

    lengthToGrow = initialLengthToGrow;
    this->initHeadPosition(sf::Vector2i(
        static_cast<int>(this->gridSizeX) / 2 - 1,
        static_cast<int>(this->gridSizeY) / 2 - 1
    ));
}

void Snake::move()
{
    sf::Vector2i head = this->getHeadPosition();

    switch (this->direction)
    {
    case Direction::RIGHT: head.x++; break;
    case Direction::LEFT:  head.x--; break;
    case Direction::UP:    head.y--; break;
    case Direction::DOWN:  head.y++; break;
    }

    // Ensure the snake stays within grid boundaries
    if (head.x >= this->gridSizeX || head.x < 0) {
        this->die();
        return;
    }
    if (head.y >= this->gridSizeY || head.y < 0) {
        this->die();
        return;
    }

    // Ensure the snake doesn't collide with itself, except for the tail (since the snake can follow its own tail)
    if (this->isCollidingAt(head) && (!this->isTailCollidingAt(head) || this->lengthToGrow > 0)) {
        this->die();
        return;
    }

    this->prevDirection = this->direction;
    this->direction = this->nextDirection;

    this->body.push_front(head);
    this->removeFromFreeTiles(head);

    if (this->lengthToGrow > 0) {
        this->lengthToGrow--;
    } else {
        if (head != this->body.back())
            this->freeTiles.push_back(this->body.back());
        this->body.pop_back();
    }
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

    this->distanceTraveled += this->getSpeedPixelsPerSec() * dt;

    if (this->distanceTraveled > this->tileSize)
    {
        this->move();
        this->distanceTraveled -= this->tileSize;
    }
}

void Snake::render(sf::RenderTarget& target, float offsetX, float offsetY)
{
    for (size_t i = 0; i < this->body.size(); ++i)
    {
        const auto& curSegment = this->body[i];

        const sf::Vector2f position(
            offsetX + this->tileSize * curSegment.x,
            offsetY + this->tileSize * curSegment.y
        );

        // render segment
        this->bodySegment.setPosition(position);
        target.draw(this->bodySegment);

        if (i == 0) {
            this->renderHeadBorder(target, position);
        } else if (i == this->body.size() - 1) {
            this->renderTailBorder(target, position, i);
        } else {
            this->renderSegmentBorder(target, position, i);
        }
    }
}

void Snake::resetFreeTiles()
{
    this->freeTiles.clear();
    // Swap to release memory and set the vector's capacity to a minimum (after clearing).
    std::vector<sf::Vector2i>(this->freeTiles).swap(this->freeTiles);
    this->freeTiles.reserve(static_cast<size_t>(this->gridSizeX) * static_cast<size_t>(this->gridSizeY));

    for (int x = 0; x < this->gridSizeX; ++x) {
        for (int y = 0; y < this->gridSizeY; ++y) {
            this->freeTiles.emplace_back(x, y);
        }
    }
}

void Snake::removeFromFreeTiles(const sf::Vector2i& position)
{
    for (size_t i = 0; i < this->freeTiles.size(); ++i) {
        if (this->freeTiles[i] == position) {
            // swap with the last element to save time
            this->freeTiles[i] = this->freeTiles.back();
            // remove swapped element from the end
            this->freeTiles.pop_back();
            break;
        }
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

void Snake::renderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position)
{
    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());
    this->renderBorder(target, BorderSide::ALL);
}

void Snake::renderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t tailIndex)
{
    assert(tailIndex >= 1 && tailIndex < this->body.size());

    const auto& curSegment = this->body[tailIndex];
    const auto& prevSegment = this->body[tailIndex - 1];

    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());

    if (prevSegment.x == curSegment.x)
    {
        renderBorder(target, BorderSide::VERTICAL);
        renderBorder(
            target,
            prevSegment.y + 1 == curSegment.y
            ? BorderSide::BOTTOM
            : BorderSide::TOP
        );
    } else if (prevSegment.y == curSegment.y)
    {
        renderBorder(target, BorderSide::HORIZONTAL);
        renderBorder(
            target,
            prevSegment.x + 1 == curSegment.x
            ? BorderSide::RIGHT
            : BorderSide::LEFT
        );
    }
}

void Snake::renderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t segmentIndex)
{
    assert(segmentIndex >= 1 && segmentIndex < this->body.size());

    const auto& curSegment = this->body[segmentIndex];
    const auto& prevSegment = this->body[segmentIndex - 1];
    const auto& nextSegment = this->body[segmentIndex + 1];

    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());
    this->bodyBorderCorner.setPosition(position + this->bodyBorderCorner.getOrigin());

    const int deltaXToPrev = prevSegment.x - curSegment.x;
    const int deltaXToNext = nextSegment.x - curSegment.x;

    const int deltaYToPrev = prevSegment.y - curSegment.y;
    const int deltaYToNext = nextSegment.y - curSegment.y;

    // Render borders based on adjacent segments
    if (deltaXToPrev == 0 && deltaXToNext == 0) {
        // Vertical segments (right & left)
        renderBorder(target, BorderSide::VERTICAL);
    } else if (deltaYToPrev == 0 && deltaYToNext == 0) {
        // Horizontal segments (top & bottom)
        renderBorder(target, BorderSide::HORIZONTAL);
    } else {
        // Diagonal segments: right-top, right-bottom, left-top, left-bottom
        if ((deltaXToPrev == -1 && deltaYToNext == -1) || (deltaXToNext == -1 && deltaYToPrev == -1))
        {
            renderBorder(target, BorderSide::RIGHT);
            renderBorder(target, BorderSide::BOTTOM);

            // Draw the border corner at the top-left
            this->bodyBorderCorner.setRotation(0.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == -1 && deltaYToNext == 1) || (deltaXToNext == -1 && deltaYToPrev == 1))
        {
            renderBorder(target, BorderSide::RIGHT);
            renderBorder(target, BorderSide::TOP);

            // Draw the border corner at the bottom-left
            this->bodyBorderCorner.setRotation(270.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == -1) || (deltaXToNext == 1 && deltaYToPrev == -1))
        {
            renderBorder(target, BorderSide::LEFT);
            renderBorder(target, BorderSide::BOTTOM);

            // Draw the border corner at the top-right
            this->bodyBorderCorner.setRotation(90.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == 1) || (deltaXToNext == 1 && deltaYToPrev == 1))
        {
            renderBorder(target, BorderSide::LEFT);
            renderBorder(target, BorderSide::TOP);

            // Draw the border corner at the bottom-right
            this->bodyBorderCorner.setRotation(180.f);
            target.draw(this->bodyBorderCorner);
        }
    }
}

void Snake::renderBorder(sf::RenderTarget &target, BorderSide side)
{
    switch (side)
    {
    case BorderSide::TOP:     this->bodyBorder.setRotation(0.f);   break;
    case BorderSide::BOTTOM:  this->bodyBorder.setRotation(180.f); break;
    case BorderSide::RIGHT:   this->bodyBorder.setRotation(90.f);  break;
    case BorderSide::LEFT:    this->bodyBorder.setRotation(270.f); break;
    case BorderSide::HORIZONTAL:
        renderBorder(target, BorderSide::TOP);
        renderBorder(target, BorderSide::BOTTOM);
        return;
    case BorderSide::VERTICAL:
        renderBorder(target, BorderSide::RIGHT);
        renderBorder(target, BorderSide::LEFT);
        return;
    case BorderSide::ALL:
        renderBorder(target, BorderSide::HORIZONTAL);
        renderBorder(target, BorderSide::VERTICAL);
        return;
    }

    target.draw(this->bodyBorder);
}
