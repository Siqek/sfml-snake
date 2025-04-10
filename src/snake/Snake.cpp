#include "stdafx.hpp"
#include "snake/Snake.hpp"

Snake::Snake(float speedTilesPerSec, unsigned int length)
    : speedTilesPerSec(speedTilesPerSec),
    direction(Direction::RIGHT), prevDirection(Direction::RIGHT), nextDirection(Direction::RIGHT),
    gridSizeX(0), gridSizeY(0), tileSize(0.f),
    lengthToGrow(std::max(0u, length - 1)), /* prevent underflow */
    distanceTraveled(0.f),
    body{}, freeTiles{}
{
    this->bodySegment.setFillColor(sf::Color(0xCCFFBDFF));
    this->bodyBorder.setFillColor(sf::Color(0x7ECA9CFF));
}

void Snake::initHeadPosition(sf::Vector2i position)
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

    this->bodyBorder.setSize(sf::Vector2f(this->tileSize, std::max(2.f, this->tileSize / 10.f)));
    this->bodyBorder.setOrigin(this->bodySegment.getSize() / 2.f);
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

    // Ensure the snake doesn't collide with itself, except for the tail (since the snake can follow its own tail)
    if (this->isCollidingAt(head) && (!this->isTailCollidingAt(head) || this->lengthToGrow > 0))
        return;

    this->prevDirection = this->direction;
    this->direction = this->nextDirection;

    this->body.push_front(head);
    this->removeFromFreeTiles(head);

    if (this->lengthToGrow > 0) {
        this->lengthToGrow--;
    } else {
        this->freeTiles.push_back(this->body.back());
        this->body.pop_back();
    }
}

bool Snake::isCollidingAt(sf::Vector2i position) const
{
    for (const auto& b : this->body) {
        if (b == position)
            return true;
    }

    return false;
}

bool Snake::isHeadCollidingAt(sf::Vector2i position) const
{
    return this->getHeadPosition() == position;
}

bool Snake::isTailCollidingAt(sf::Vector2i position) const
{
    return this->getTailPosition() == position;
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
    this->freeTiles.reserve(this->gridSizeX * this->gridSizeY);

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

    // Render borders based on adjacent segments
    if (prevSegment.x == curSegment.x && nextSegment.x == curSegment.x) {
        // Vertical segments (right & left)
        renderBorder(target, BorderSide::VERTICAL);
    } else if (prevSegment.y == curSegment.y && nextSegment.y == curSegment.y) {
        // Horizontal segments (top & bottom)
        renderBorder(target, BorderSide::HORIZONTAL);
    } else {
        // Diagonal segments: right-top, right-bottom, left-top, left-bottom
        if ((prevSegment.x + 1 == curSegment.x && nextSegment.y + 1 == curSegment.y) ||
            (nextSegment.x + 1 == curSegment.x && prevSegment.y + 1 == curSegment.y))
        {
            renderBorder(target, BorderSide::RIGHT);
            renderBorder(target, BorderSide::BOTTOM);
        } else if ((prevSegment.x + 1 == curSegment.x && nextSegment.y - 1 == curSegment.y) ||
                   (nextSegment.x + 1 == curSegment.x && prevSegment.y - 1 == curSegment.y))
        {
            renderBorder(target, BorderSide::RIGHT);
            renderBorder(target, BorderSide::TOP);
        } else if ((prevSegment.x - 1 == curSegment.x && nextSegment.y + 1 == curSegment.y) ||
                   (nextSegment.x - 1 == curSegment.x && prevSegment.y + 1 == curSegment.y))
        {
            renderBorder(target, BorderSide::LEFT);
            renderBorder(target, BorderSide::BOTTOM);
        } else if ((prevSegment.x - 1 == curSegment.x && nextSegment.y - 1 == curSegment.y) ||
                   (nextSegment.x - 1 == curSegment.x && prevSegment.y - 1 == curSegment.y))
        {
            renderBorder(target, BorderSide::LEFT);
            renderBorder(target, BorderSide::TOP);
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
