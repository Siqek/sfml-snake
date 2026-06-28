#include "stdafx.hpp"
#include "game/snake/BasicSnake.hpp"

#include "game/grid/Grid.hpp"

#include "config/Colors.hpp"

BasicSnake::BasicSnake(float speedTilesPerSec, unsigned length, std::shared_ptr<IGrid> grid)
    : SnakeBase(speedTilesPerSec, length, grid),
      Body{}
{
    InitHeadPosition();
}

bool BasicSnake::HasFilledGrid() const
{
    return Body.size() == Grid->GetTotalTileCount();
}

sf::Vector2i BasicSnake::GetHeadPosition() const
{
    return Body.front();
}

sf::Vector2i BasicSnake::GetTailPosition() const
{
    return Body.back();
}

void BasicSnake::Move()
{
    const sf::Vector2i newHeadPos = GetHeadPosition() + DirectionToVector(CurrentDirection);
    const sf::Vector2i nextNewHeadPos = newHeadPos + DirectionToVector(NextDirection);

    PrevDirection = CurrentDirection;
    CurrentDirection = NextDirection;
    NextDirection = NextNextDirection;

    // TODO(siqek): TEMP(siqek):
    // temporarily dissallow following tail
    const bool canMoveIntoVacatedTailTile = false;// = GetPendingGrowth() == 0 && IsTailCollidingAt(newHeadPos);

    if (canMoveIntoVacatedTailTile)
    {
        Body.pop_back();
        Body.push_front(newHeadPos);
        return;
    }

    AddHead(newHeadPos);

    if (!ConsumePendingGrowth())
    {
        RemoveTail();
    }

    if (!Grid->IsWithinBoundaries(nextNewHeadPos))
    {
        Die();
        return;
    }

    if (IsCollidingAt(nextNewHeadPos) && !canMoveIntoVacatedTailTile)
    {
        Die();
    }
}

void BasicSnake::InitHeadPosition()
{
    if (Body.empty())
    {
        AddHead(Grid->GetSnakeHeadSpawnPosition());
    }
}

bool BasicSnake::IsCollidingAt(const sf::Vector2i& position) const
{
    for (const auto& b : Body)
    {
        if (b == position)
        {
            return true;
        }
    }
    return false;
}

bool BasicSnake::IsHeadCollidingAt(const sf::Vector2i& position) const
{
    return GetHeadPosition() == position;
}

bool BasicSnake::IsTailCollidingAt(const sf::Vector2i &position) const
{
    return GetTailPosition() == position;
}

void BasicSnake::AddHead(const sf::Vector2i& head)
{
    if (!Grid->OccupyTile(head))
    {
        throw std::runtime_error("Tile is already taken.");
    }
    Body.push_front(head);
}

void BasicSnake::RemoveTail()
{
    if (!Grid->FreeTile(Body.back()))
    {
        throw std::runtime_error("Tile is already freed.");
    }
    Body.pop_back();
}
