#ifndef SNAKERENDERER_HPP
#define SNAKERENDERER_HPP

#include "snake/Snake.hpp"

class SnakeRenderer
{
public:
    SnakeRenderer();

    void setTileSize(float size);

    void render(sf::RenderTarget& target, const Snake& snake, const sf::Vector2f& offset);

private:
    enum class BorderSide {
        Top,
        Bottom,
        Right,
        Left,
        Horizontal,
        Vertical,
        All
    };

    void renderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position);
    void renderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current);
    void renderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, const sf::Vector2i& previous, const sf::Vector2i& current, const sf::Vector2i& next);

    void renderBorder(sf::RenderTarget& target, BorderSide side);

    float tileSize;

    sf::RectangleShape bodySegment;
    sf::RectangleShape bodyBorder;
    sf::RectangleShape bodyBorderCorner;
};

#endif // SNAKERENDERER_HPP
