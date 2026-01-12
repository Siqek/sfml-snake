#ifndef BASICSNAKE_HPP
#define BASICSNAKE_HPP

class IGrid;

#include "game/snake/Snake.hpp"

class BasicSnake
    : public SnakeBase
{
public:
    BasicSnake(float speedTilesPerSec, unsigned length, std::shared_ptr<IGrid> grid);
    ~BasicSnake() override = default;

    bool HasFilledGrid() const override;

    sf::Vector2i GetHeadPosition() const override;
    sf::Vector2i GetTailPosition() const override;

    const std::deque<sf::Vector2i>& GetBody() const override { return Body; }

private:
    void Move() override;

    void InitHeadPosition();

    bool IsCollidingAt(const sf::Vector2i& position) const;
    bool IsHeadCollidingAt(const sf::Vector2i& position) const;
    bool IsTailCollidingAt(const sf::Vector2i& position) const;

    void AddHead(const sf::Vector2i& head);

    void RemoveTail();

    std::deque<sf::Vector2i> Body;
};

#endif // BASICSNAKE_HPP
