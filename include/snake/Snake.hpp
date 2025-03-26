#ifndef SNAKE_HPP
#define SNAKE_HPP

enum class Direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

using Position = std::pair<int, int>;

class Snake
{
public:
    Snake(float speed, unsigned int length);
    ~Snake() = default;

    Position getHeadPosition() const;

    void setGridSize(uint8_t x, uint8_t y);
    void setTileSize(float size);

    void setDirection(Direction direction);
    void grow(unsigned int length);
    void move();

    bool checkCollision(Position position);

    void update(const float& dt);
    void render(sf::RenderTarget& target);

private:
    float speed;
    Direction direction;

    uint8_t gridSizeX;
    uint8_t gridSizeY;
    float tileSize;

    unsigned int lengthToGrow;
    float distanceTraveled;

    std::deque<Position> body;
};

#endif // SNAKE_HPP
