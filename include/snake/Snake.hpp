#ifndef SNAKE_HPP
#define SNAKE_HPP

class Grid;

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
    Snake(float speed, unsigned int length, uint8_t gridSizeX, uint8_t gridSizeY, Grid* grid);
    ~Snake() = default;

    sf::Vector2i getHeadPosition() const { return this->body.front(); };
    sf::Vector2i getTailPosition() const { return this->body.back(); };
    float getSpeedPixelsPerSec() const { return this->speedTilesPerSec * this->tileSize; };

    const std::vector<sf::Vector2i>& getFreeTiles() const { return this->freeTiles; };

    bool getIsAlive() const { return isAlive; };
    bool hasFilledGrid() const { return freeTiles.size() == 0; };

    void setTileSize(float size);

    void setDirection(Direction direction);
    void grow(unsigned int lengthToGrow);

    void reset();
    void resetAndResizeGrid(uint8_t x, uint8_t y);

    bool isCollidingAt(const sf::Vector2i& position) const;
    bool isHeadCollidingAt(const sf::Vector2i& position) const { return getHeadPosition() == position; };
    bool isTailCollidingAt(const sf::Vector2i& position) const { return getTailPosition() == position; };

    void update(const float& dt);
    void render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

private:
    static constexpr uint8_t MinGridSize = 4;

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

    Grid* grid;

    uint8_t gridSizeX;
    uint8_t gridSizeY;
    float tileSize;

    unsigned int lengthToGrow;
    unsigned int initialLengthToGrow;

    float distanceTraveled;

    sf::RectangleShape bodySegment;
    sf::RectangleShape bodyBorder;
    sf::RectangleShape bodyBorderCorner;

    std::deque<sf::Vector2i> body;
    std::vector<sf::Vector2i> freeTiles;

    bool isAlive;

    void initHeadPosition(const sf::Vector2i& position);

    void die() { isAlive = false; };

    void move();

    void addHead(const sf::Vector2i& head);
    void removeTail();

    Direction getOppositeDirection(Direction direction) const;

    void setGridSize(uint8_t x, uint8_t y);

    void resetFreeTiles();
    void removeFromFreeTiles(const sf::Vector2i& position);

    void renderHeadBorder(sf::RenderTarget& target, const sf::Vector2f& position);
    void renderTailBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t tailIndex);
    void renderSegmentBorder(sf::RenderTarget& target, const sf::Vector2f& position, size_t segmentIndex);

    void renderBorder(sf::RenderTarget& target, BorderSide side);
};

#endif // SNAKE_HPP
