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

    virtual bool IsAlive() const = 0;

    virtual bool HasFilledGrid() const = 0;

    virtual sf::Vector2i GetHeadPosition() const = 0;

    virtual sf::Vector2i GetTailPosition() const = 0;

    virtual const std::deque<sf::Vector2i>& GetBody() const = 0;

    virtual void ChangeDirection(EMoveDirection direction) = 0;

    virtual void Grow(unsigned lengthToGrow) = 0;

    virtual void Update(float dt) = 0;
};

class SnakeBase
    : public ISnake
{
public:
    virtual ~SnakeBase() override = default;

    bool IsAlive() const override { return bIsAlive; }

    void ChangeDirection(EMoveDirection direction) override;

    void Grow(unsigned lengthToGrow) override;

    virtual void Update(float dt) override;

protected:
    SnakeBase(float speedTilesPerSec, unsigned length, std::shared_ptr<IGrid> grid);

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
