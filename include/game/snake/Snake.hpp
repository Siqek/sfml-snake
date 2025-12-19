#ifndef SNAKE_HPP
#define SNAKE_HPP

class IGrid;

enum class EMoveDirection
{
    Right,
    Left,
    Up,
    Down
};

class ISnake
{
public:
    virtual ~ISnake() = default;

    bool IsAlive() const { return bIsAlive; }

    virtual bool HasFilledGrid() const = 0;

    virtual sf::Vector2i GetHeadPosition() const = 0;
    virtual sf::Vector2i GetTailPosition() const = 0;

    virtual const std::deque<sf::Vector2i>& GetBody() const = 0;

    void ChangeDirection(EMoveDirection direction);

    void Grow(unsigned lengthToGrow);

    virtual void Update(float dt);

protected:
    ISnake(float speedTilesPerSec, unsigned length, std::shared_ptr<IGrid> grid);

    unsigned GetPendingGrowth() const { return PendingGrowth; }

    bool ConsumePendingGrowth();

    virtual void Move() = 0;

    void Die();

    static EMoveDirection OppositeDirectionTo(EMoveDirection direction);

    std::shared_ptr<IGrid> Grid;

    EMoveDirection CurrentDirection;
    EMoveDirection PrevDirection;
    EMoveDirection NextDirection;

private:
    float SpeedTilesPerSec;
    unsigned PendingGrowth;

    float TilesTraveled;

    bool bIsAlive;
};

#endif // SNAKE_HPP
