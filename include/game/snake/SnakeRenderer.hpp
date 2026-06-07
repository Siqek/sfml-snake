#ifndef SNAKERENDERER_HPP
#define SNAKERENDERER_HPP

class ISnake;

class RenderContext;

class SnakeRenderer
{
public:
    SnakeRenderer();

    void SetTileSize(float size);

    void Render(sf::RenderTarget& target, const ISnake& snake, const sf::Vector2f& offset);

    void FillContext(RenderContext& context, const ISnake& snake, const sf::Vector2f& offset);

private:
    enum class EBorderSide {
        Top,
        Bottom,
        Right,
        Left,
        Horizontal,
        Vertical,
        All
    };

    void RenderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position);
    void RenderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current);
    void RenderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next);

    void RenderBorder(sf::RenderTarget& target, EBorderSide side);

    void AddHeadBorderToContext(RenderContext& target, const sf::Vector2f& position);
    void AddTailBorderToContext(RenderContext& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current);
    void AddSegmentBorderToContext(RenderContext& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next);

    void AddBorderToContext(RenderContext& context, EBorderSide side);

    float TileSize;

    sf::RectangleShape BodySegment;
    sf::RectangleShape BodyBorder;
    sf::RectangleShape BodyBorderCorner;
};

#endif // SNAKERENDERER_HPP
