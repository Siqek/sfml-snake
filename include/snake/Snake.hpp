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
    float getSpeedPixelsPerSec() const { return this->speedTilesPerSec * this->tileSize; };

    std::vector<sf::Vector2i> getUnoccupiedTiles() const;

    void setGridSize(uint8_t x, uint8_t y);
    void setTileSize(float size);

    void setDirection(Direction direction);
    void grow(unsigned int lengthToGrow);
    void move();

    bool checkCollision(sf::Vector2i position) const;
    bool checkHeadCollision(sf::Vector2i position) const;

    void update(const float& dt);
    void render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

private:
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

    Direction getOppositeDirection(Direction direction) const;
};

#endif // SNAKE_HPP
