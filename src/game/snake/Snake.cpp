#include "stdafx.hpp"
#include "game/snake/Snake.hpp"

SnakeBase::SnakeBase(float speedTilesPerSec, unsigned length, std::shared_ptr<IGrid> grid)
    : Grid(grid),
      SpeedTilesPerSec(speedTilesPerSec),
      PendingGrowth(std::max(1u, length) - 1u),
      TilesTraveled(0.f),
      bIsAlive(true)
{
    assert(speedTilesPerSec > 0.f);
    assert(length != 0);
    assert(grid != nullptr);
}

void SnakeBase::ChangeDirection(EMoveDirection direction)
{
    const bool isDirectionUnchanged = CurrentDirection == NextDirection;

    const EMoveDirection referenceDirection = isDirectionUnchanged ? CurrentDirection : NextDirection;
    const bool isTryingToSetOppositeDirection = direction == OppositeDirectionTo(referenceDirection);

    if (isTryingToSetOppositeDirection)
    {
        return;
    }

    if (isDirectionUnchanged)
    {
        NextDirection = direction;
    }

    NextNextDirection = direction;
}

void SnakeBase::Grow(unsigned lengthToGrow)
{
    PendingGrowth += lengthToGrow;
}

void SnakeBase::Update(float dt)
{
    if (HasFilledGrid() || !IsAlive())
    {
        return;
    }

    TilesTraveled += SpeedTilesPerSec * dt;

    while (TilesTraveled >= 1.f)
    {
        Move();
        TilesTraveled -= 1.f;
    }
}

bool SnakeBase::ConsumePendingGrowth()
{
    if (PendingGrowth > 0)
    {
        PendingGrowth--;
        return true;
    }
    return false;
}

void SnakeBase::Die()
{
    bIsAlive = false;
}

EMoveDirection SnakeBase::OppositeDirectionTo(EMoveDirection direction)
{
    switch (direction)
    {
        case EMoveDirection::Right: return EMoveDirection::Left;
        case EMoveDirection::Left:  return EMoveDirection::Right;
        case EMoveDirection::Down:  return EMoveDirection::Up;
        case EMoveDirection::Up:    return EMoveDirection::Down;
    }
    return direction;
}
