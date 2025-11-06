#include "stdafx.hpp"
#include "snake/SnakeRenderer.hpp"

#include "config/Colors.hpp"

SnakeRenderer::SnakeRenderer()
{
    this->bodySegment.setFillColor(sf::Color(Colors::Hex::SnakeBody));
    this->bodyBorder.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
    this->bodyBorderCorner.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
}

void SnakeRenderer::setTileSize(float size)
{
    this->tileSize = size;
    this->bodySegment.setSize(sf::Vector2f(this->tileSize, this->tileSize));

    const auto borderThickness = std::max(2.f, this->tileSize / 10.f);

    this->bodyBorder.setSize(sf::Vector2f(this->tileSize, borderThickness));
    this->bodyBorder.setOrigin(this->bodySegment.getSize() / 2.f);

    this->bodyBorderCorner.setSize(sf::Vector2f(borderThickness, borderThickness));
    this->bodyBorderCorner.setOrigin(this->bodySegment.getSize() / 2.f);
}

void SnakeRenderer::render(sf::RenderTarget& target, const Snake& snake, const sf::Vector2f& offset)
{
    const auto& snakeBody = snake.getBody();
    for (size_t i = 0; i < snakeBody.size(); ++i)
    {
        const sf::Vector2f position = offset + sf::Vector2f(snakeBody[i]) * this->tileSize;

        this->bodySegment.setPosition(position);
        target.draw(this->bodySegment);

        if (i == 0) {
            this->renderHeadBorder(target, position);
            continue;
        }

        if (i == snakeBody.size() - 1) {
            this->renderTailBorder(target, position, snakeBody[i - 1], snakeBody[i]);
            continue;
        }

        this->renderSegmentBorder(target, position, snakeBody[i - 1], snakeBody[i], snakeBody[i + 1]);
    }
}

void SnakeRenderer::renderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position)
{
    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());
    this->renderBorder(target, BorderSide::All);
}

void SnakeRenderer::renderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current)
{
    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());

    if (previous.x == current.x)
    {
        renderBorder(target, BorderSide::Vertical);
        renderBorder(
            target,
            previous.y + 1 == current.y
            ? BorderSide::Bottom
            : BorderSide::Top
        );
    } else if (previous.y == current.y)
    {
        renderBorder(target, BorderSide::Horizontal);
        renderBorder(
            target,
            previous.x + 1 == current.x
            ? BorderSide::Right
            : BorderSide::Left
        );
    }
}


void SnakeRenderer::renderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next)
{
    this->bodyBorder.setPosition(position + this->bodyBorder.getOrigin());
    this->bodyBorderCorner.setPosition(position + this->bodyBorderCorner.getOrigin());

    const int deltaXToPrev = previous.x - current.x;
    const int deltaXToNext = next.x - current.x;

    const int deltaYToPrev = previous.y - current.y;
    const int deltaYToNext = next.y - current.y;

    // Render borders based on adjacent segments
    if (deltaXToPrev == 0 && deltaXToNext == 0) {
        // Vertical segments (right & left)
        renderBorder(target, BorderSide::Vertical);
    } else if (deltaYToPrev == 0 && deltaYToNext == 0) {
        // Horizontal segments (top & bottom)
        renderBorder(target, BorderSide::Horizontal);
    } else {
        // Diagonal segments: right-top, right-bottom, left-top, left-bottom
        if ((deltaXToPrev == -1 && deltaYToNext == -1) || (deltaXToNext == -1 && deltaYToPrev == -1))
        {
            renderBorder(target, BorderSide::Right);
            renderBorder(target, BorderSide::Bottom);

            // Draw the border corner at the top-left
            this->bodyBorderCorner.setRotation(0.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == -1 && deltaYToNext == 1) || (deltaXToNext == -1 && deltaYToPrev == 1))
        {
            renderBorder(target, BorderSide::Right);
            renderBorder(target, BorderSide::Top);

            // Draw the border corner at the bottom-left
            this->bodyBorderCorner.setRotation(270.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == -1) || (deltaXToNext == 1 && deltaYToPrev == -1))
        {
            renderBorder(target, BorderSide::Left);
            renderBorder(target, BorderSide::Bottom);

            // Draw the border corner at the top-right
            this->bodyBorderCorner.setRotation(90.f);
            target.draw(this->bodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == 1) || (deltaXToNext == 1 && deltaYToPrev == 1))
        {
            renderBorder(target, BorderSide::Left);
            renderBorder(target, BorderSide::Top);

            // Draw the border corner at the bottom-right
            this->bodyBorderCorner.setRotation(180.f);
            target.draw(this->bodyBorderCorner);
        }
    }
}

void SnakeRenderer::renderBorder(sf::RenderTarget &target, BorderSide side)
{
    switch (side)
    {
    case BorderSide::Top:     this->bodyBorder.setRotation(0.f);   break;
    case BorderSide::Bottom:  this->bodyBorder.setRotation(180.f); break;
    case BorderSide::Right:   this->bodyBorder.setRotation(90.f);  break;
    case BorderSide::Left:    this->bodyBorder.setRotation(270.f); break;
    case BorderSide::Horizontal:
        renderBorder(target, BorderSide::Top);
        renderBorder(target, BorderSide::Bottom);
        return;
    case BorderSide::Vertical:
        renderBorder(target, BorderSide::Right);
        renderBorder(target, BorderSide::Left);
        return;
    case BorderSide::All:
        renderBorder(target, BorderSide::Horizontal);
        renderBorder(target, BorderSide::Vertical);
        return;
    }

    target.draw(this->bodyBorder);
}

