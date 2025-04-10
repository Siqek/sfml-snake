#ifndef SNAKE_HPP
#define SNAKE_HPP

enum class Direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

class Snake
{
public:
    Snake(float speed, unsigned int length);
    ~Snake() = default;

    void initHeadPosition(sf::Vector2i position);

    sf::Vector2i getHeadPosition() const { return this->body.front(); };
    sf::Vector2i getTailPosition() const { return this->body.back(); };
    float getSpeedPixelsPerSec() const { return this->speedTilesPerSec * this->tileSize; };

    const std::vector<sf::Vector2i>& getFreeTiles() const { return this->freeTiles; };

    void setGridSize(uint8_t x, uint8_t y);
    void setTileSize(float size);

    void setDirection(Direction direction);
    void grow(unsigned int lengthToGrow);
    void move();

    bool isCollidingAt(sf::Vector2i position) const;
    bool isHeadCollidingAt(sf::Vector2i position) const;
    bool isTailCollidingAt(sf::Vector2i position) const;

    void update(const float& dt);
    void render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

private:
    enum class BorderSide {
        TOP,
        BOTTOM,
        RIGHT,
        LEFT,
        HORIZONTAL,
        VERTICAL,
        ALL
    };

    float speedTilesPerSec;
    Direction direction;
    Direction prevDirection;
    Direction nextDirection;

    uint8_t gridSizeX;
    uint8_t gridSizeY;
    float tileSize;

    unsigned int lengthToGrow;
    float distanceTraveled;

    sf::RectangleShape bodySegment;
    sf::RectangleShape bodyBorder;
    std::deque<sf::Vector2i> body;
    std::vector<sf::Vector2i> freeTiles;

    Direction getOppositeDirection(Direction direction) const;

    void resetFreeTiles();
    void removeFromFreeTiles(const sf::Vector2i& position);

    void renderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position);
    void renderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t tailIndex);
    void renderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t segmentIndex);

    void renderBorder(sf::RenderTarget& target, BorderSide side);
};

#endif // SNAKE_HPP
