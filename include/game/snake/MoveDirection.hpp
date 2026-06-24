#ifndef MOVEDIRECTION_HPP
#define MOVEDIRECTION_HPP

#include <SFML/System/Vector2.hpp>

enum class EMoveDirection
{
    Right,
    Left,
    Up,
    Down
};

inline sf::Vector2i DirectionToVector(EMoveDirection direction)
{
    switch (direction)
    {
        case EMoveDirection::Right: return sf::Vector2i{  1,  0 };
        case EMoveDirection::Left:  return sf::Vector2i{ -1,  0 };
        case EMoveDirection::Up:    return sf::Vector2i{  0, -1 };
        case EMoveDirection::Down:  return sf::Vector2i{  0,  1 };
    }
}

#endif // MOVEDIRECTION_HPP
