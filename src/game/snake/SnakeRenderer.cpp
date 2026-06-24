#include "stdafx.hpp"
#include "game/snake/SnakeRenderer.hpp"

#include "game/snake/Snake.hpp"
#include "game/snake/MoveDirection.hpp"

#include "config/Colors.hpp"

#include "render/RenderContext.hpp"

SnakeRenderer::SnakeRenderer()
{
    BodySegment.setFillColor(sf::Color(Colors::Hex::SnakeBody));
    BodyBorder.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
    BodyBorderCorner.setFillColor(sf::Color(Colors::Hex::SnakeOutline));
}

void SnakeRenderer::SetTileSize(float size)
{
    TileSize = size;
    BodySegment.setSize(sf::Vector2f(TileSize, TileSize));

    const auto borderThickness = std::max(2.f, TileSize / 10.f);

    BodyBorder.setSize(sf::Vector2f(TileSize, borderThickness));
    BodyBorder.setOrigin(BodySegment.getSize() / 2.f);

    BodyBorderCorner.setSize(sf::Vector2f(borderThickness, borderThickness));
    BodyBorderCorner.setOrigin(BodySegment.getSize() / 2.f);
}

void SnakeRenderer::Render(sf::RenderTarget& target, const ISnake& snake, const sf::Vector2f& offset)
{
    const auto& snakeBody = snake.GetBody();
    for (size_t i = 0; i < snakeBody.size(); ++i)
    {
        const sf::Vector2f position = offset + sf::Vector2f(snakeBody[i]) * TileSize;

        BodySegment.setPosition(position);
        target.draw(BodySegment);

        if (i == 0)
        {
            RenderHeadBorder(target, position);
            continue;
        }

        if (i == snakeBody.size() - 1)
        {
            RenderTailBorder(target, position, snakeBody[i - 1], snakeBody[i]);
            continue;
        }

        RenderSegmentBorder(target, position, snakeBody[i - 1], snakeBody[i], snakeBody[i + 1]);
    }
}

void SnakeRenderer::FillContext(RenderContext& context, const ISnake& snake, const sf::Vector2f& offset)
{
    const auto& snakeBody = snake.GetBody();
    for (size_t i = 1; i < snakeBody.size(); ++i)
    {
        const sf::Vector2f position = offset + sf::Vector2f(snakeBody[i]) * TileSize;

        BodySegment.setPosition(position);
        context.Drawables.emplace_back(BodySegment);

        if (i == snakeBody.size() - 1)
        {
            AddTailBorderToContext(context, position, snakeBody[i - 1], snakeBody[i]);
            continue;
        }

        AddSegmentBorderToContext(context, position, snakeBody[i - 1], snakeBody[i], snakeBody[i + 1]);
    }

    if (snakeBody.size() > 0)
    {
        const sf::Vector2f position = offset + sf::Vector2f(snakeBody[0]) * TileSize;

        BodySegment.setPosition(position);
        context.Drawables.emplace_back(BodySegment);

        // TODO(siqek): head animation - movement between tiles

        // const sf::Vector2f moveProgress = sf::Vector2f(DirectionToVector(snake.GetDirection())) * std::min(snake.GetTilesTraveled(), 1.f) * TileSize;
        // BodySegment.setPosition(position + moveProgress);
        // context.Drawables.emplace_back(BodySegment);

        AddHeadBorderToContext(context, position);
    }
}

void SnakeRenderer::RenderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());
    RenderBorder(target, EBorderSide::All);
}

void SnakeRenderer::RenderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());

    if (previous.x == current.x)
    {
        RenderBorder(target, EBorderSide::Vertical);
        RenderBorder(
            target,
            previous.y + 1 == current.y
            ? EBorderSide::Bottom
            : EBorderSide::Top
        );
    } else if (previous.y == current.y)
    {
        RenderBorder(target, EBorderSide::Horizontal);
        RenderBorder(
            target,
            previous.x + 1 == current.x
            ? EBorderSide::Right
            : EBorderSide::Left
        );
    }
}


void SnakeRenderer::RenderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());
    BodyBorderCorner.setPosition(position + BodyBorderCorner.getOrigin());

    const int deltaXToPrev = previous.x - current.x;
    const int deltaXToNext = next.x - current.x;

    const int deltaYToPrev = previous.y - current.y;
    const int deltaYToNext = next.y - current.y;

    // Render borders based on adjacent segments
    if (deltaXToPrev == 0 && deltaXToNext == 0) {
        // Vertical segments (right & left)
        RenderBorder(target, EBorderSide::Vertical);
    } else if (deltaYToPrev == 0 && deltaYToNext == 0) {
        // Horizontal segments (top & bottom)
        RenderBorder(target, EBorderSide::Horizontal);
    } else {
        // Diagonal segments: right-top, right-bottom, left-top, left-bottom
        if ((deltaXToPrev == -1 && deltaYToNext == -1) || (deltaXToNext == -1 && deltaYToPrev == -1))
        {
            RenderBorder(target, EBorderSide::Right);
            RenderBorder(target, EBorderSide::Bottom);

            // Draw the border corner at the top-left
            BodyBorderCorner.setRotation(0.f);
            target.draw(BodyBorderCorner);
        }
        else if ((deltaXToPrev == -1 && deltaYToNext == 1) || (deltaXToNext == -1 && deltaYToPrev == 1))
        {
            RenderBorder(target, EBorderSide::Right);
            RenderBorder(target, EBorderSide::Top);

            // Draw the border corner at the bottom-left
            BodyBorderCorner.setRotation(270.f);
            target.draw(BodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == -1) || (deltaXToNext == 1 && deltaYToPrev == -1))
        {
            RenderBorder(target, EBorderSide::Left);
            RenderBorder(target, EBorderSide::Bottom);

            // Draw the border corner at the top-right
            BodyBorderCorner.setRotation(90.f);
            target.draw(BodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == 1) || (deltaXToNext == 1 && deltaYToPrev == 1))
        {
            RenderBorder(target, EBorderSide::Left);
            RenderBorder(target, EBorderSide::Top);

            // Draw the border corner at the bottom-right
            BodyBorderCorner.setRotation(180.f);
            target.draw(BodyBorderCorner);
        }
    }
}

void SnakeRenderer::RenderBorder(sf::RenderTarget &target, EBorderSide side)
{
    switch (side)
    {
        case EBorderSide::Top:     BodyBorder.setRotation(0.f);   break;
        case EBorderSide::Bottom:  BodyBorder.setRotation(180.f); break;
        case EBorderSide::Right:   BodyBorder.setRotation(90.f);  break;
        case EBorderSide::Left:    BodyBorder.setRotation(270.f); break;
        case EBorderSide::Horizontal:
            RenderBorder(target, EBorderSide::Top);
            RenderBorder(target, EBorderSide::Bottom);
            return;
        case EBorderSide::Vertical:
            RenderBorder(target, EBorderSide::Right);
            RenderBorder(target, EBorderSide::Left);
            return;
        case EBorderSide::All:
            RenderBorder(target, EBorderSide::Horizontal);
            RenderBorder(target, EBorderSide::Vertical);
            return;
    }

    target.draw(BodyBorder);
}

void SnakeRenderer::AddHeadBorderToContext(RenderContext& context, const sf::Vector2f& position)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());
    AddBorderToContext(context, EBorderSide::All);
}

void SnakeRenderer::AddTailBorderToContext(RenderContext& context, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());

    if (previous.x == current.x)
    {
        const EBorderSide borderSide = previous.y + 1 == current.y ? EBorderSide::Bottom : EBorderSide::Top;
        AddBorderToContext(context, EBorderSide::Vertical);
        AddBorderToContext(context, borderSide);
    }
    else if (previous.y == current.y)
    {
        const EBorderSide borderSide = previous.x + 1 == current.x ? EBorderSide::Right : EBorderSide::Left;
        AddBorderToContext(context, EBorderSide::Horizontal);
        AddBorderToContext(context, borderSide);
    }
}


void SnakeRenderer::AddSegmentBorderToContext(RenderContext& context, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next)
{
    BodyBorder.setPosition(position + BodyBorder.getOrigin());
    BodyBorderCorner.setPosition(position + BodyBorderCorner.getOrigin());

    const int deltaXToPrev = previous.x - current.x;
    const int deltaXToNext = next.x - current.x;

    const int deltaYToPrev = previous.y - current.y;
    const int deltaYToNext = next.y - current.y;

    // Render borders based on adjacent segments
    if (deltaXToPrev == 0 && deltaXToNext == 0) {
        // Vertical segments (right & left)
        AddBorderToContext(context, EBorderSide::Vertical);
    } else if (deltaYToPrev == 0 && deltaYToNext == 0) {
        // Horizontal segments (top & bottom)
        AddBorderToContext(context, EBorderSide::Horizontal);
    } else {
        // Diagonal segments: right-top, right-bottom, left-top, left-bottom
        if ((deltaXToPrev == -1 && deltaYToNext == -1) || (deltaXToNext == -1 && deltaYToPrev == -1))
        {
            AddBorderToContext(context, EBorderSide::Right);
            AddBorderToContext(context, EBorderSide::Bottom);

            // Draw the border corner at the top-left
            BodyBorderCorner.setRotation(0.f);
            context.Drawables.emplace_back(BodyBorderCorner);
        }
        else if ((deltaXToPrev == -1 && deltaYToNext == 1) || (deltaXToNext == -1 && deltaYToPrev == 1))
        {
            AddBorderToContext(context, EBorderSide::Right);
            AddBorderToContext(context, EBorderSide::Top);

            // Draw the border corner at the bottom-left
            BodyBorderCorner.setRotation(270.f);
            context.Drawables.emplace_back(BodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == -1) || (deltaXToNext == 1 && deltaYToPrev == -1))
        {
            AddBorderToContext(context, EBorderSide::Left);
            AddBorderToContext(context, EBorderSide::Bottom);

            // Draw the border corner at the top-right
            BodyBorderCorner.setRotation(90.f);
            context.Drawables.emplace_back(BodyBorderCorner);
        }
        else if ((deltaXToPrev == 1 && deltaYToNext == 1) || (deltaXToNext == 1 && deltaYToPrev == 1))
        {
            AddBorderToContext(context, EBorderSide::Left);
            AddBorderToContext(context, EBorderSide::Top);

            // Draw the border corner at the bottom-right
            BodyBorderCorner.setRotation(180.f);
            context.Drawables.emplace_back(BodyBorderCorner);
        }
    }
}

void SnakeRenderer::AddBorderToContext(RenderContext& context, EBorderSide side)
{
    switch (side)
    {
        case EBorderSide::Top:     BodyBorder.setRotation(0.f);   break;
        case EBorderSide::Bottom:  BodyBorder.setRotation(180.f); break;
        case EBorderSide::Right:   BodyBorder.setRotation(90.f);  break;
        case EBorderSide::Left:    BodyBorder.setRotation(270.f); break;
        case EBorderSide::Horizontal:
            AddBorderToContext(context, EBorderSide::Top);
            AddBorderToContext(context, EBorderSide::Bottom);
            return;
        case EBorderSide::Vertical:
            AddBorderToContext(context, EBorderSide::Right);
            AddBorderToContext(context, EBorderSide::Left);
            return;
        case EBorderSide::All:
            AddBorderToContext(context, EBorderSide::Horizontal);
            AddBorderToContext(context, EBorderSide::Vertical);
            return;
    }

    context.Drawables.emplace_back(BodyBorder);
}
