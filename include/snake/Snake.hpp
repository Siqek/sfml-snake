#ifndef SNAKE_HPP
#define SNAKE_HPP

class IGrid;

enum class Direction
{
    Right,
    Left,
    Up,
    Down
};

class Snake
{
public:
    Snake(float speed, unsigned int length, std::shared_ptr<IGrid> grid);
    ~Snake() = default;

    sf::Vector2i getHeadPosition() const { return body.front(); }
    sf::Vector2i getTailPosition() const { return body.back(); }

    const std::deque<sf::Vector2i>& getBody() const { return body; }

    bool getIsAlive() const { return isAlive; };
    bool hasFilledGrid() const;

    void setDirection(Direction direction);
    void grow(unsigned int lengthToGrow);

    void reset();

    bool isCollidingAt(const sf::Vector2i& position) const;
    bool isHeadCollidingAt(const sf::Vector2i& position) const { return getHeadPosition() == position; };
    bool isTailCollidingAt(const sf::Vector2i& position) const { return getTailPosition() == position; };

    void update(const float& dt);

private:
    void initHeadPosition(const sf::Vector2i& position);

    void die() { isAlive = false; };

    void move();

    void addHead(const sf::Vector2i& head);
    void removeTail();

    Direction getOppositeDirection(Direction direction) const;

    float speedTilesPerSec;
    Direction direction;
    Direction prevDirection;
    Direction nextDirection;

    std::shared_ptr<IGrid> grid;

    unsigned int lengthToGrow;
    unsigned int initialLengthToGrow;

    float tilesTraveled;

    std::deque<sf::Vector2i> body;

    bool isAlive;
};

#endif // SNAKE_HPP
