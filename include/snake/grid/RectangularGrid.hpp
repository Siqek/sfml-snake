#ifndef RECTANGULARGRID_HPP
#define RECTANGULARGRID_HPP

#include "snake/grid/Grid.hpp"

class RectangularGrid
    : public IGrid
{
public:
    RectangularGrid(const sf::Vector2i& size);
    virtual ~RectangularGrid() = default;

    virtual bool IsWithinBoundaries(const sf::Vector2i& position) const override;

    bool IsOccupied(const sf::Vector2i& position) const override;

    const sf::Vector2i& GetSize() const override { return Size; }

    const std::vector<sf::Vector2i>& GetFreeTiles() const override { return FreeTiles; }

    virtual size_t GetTotalTileCount() const override;

    bool FreeTile(const sf::Vector2i& position) override;
    bool OccupyTile(const sf::Vector2i& position) override;

protected:
    enum class EInitMode {
        Full,
        Partial // derived class completes the initialization
    };

    RectangularGrid(const sf::Vector2i& size, EInitMode initMode);

    void InitFreeTiles();

    static constexpr int MinGridSize = 4;

private:
    void RemoveFromFreeTiles(const sf::Vector2i& position);

    sf::Vector2i Size;

    std::vector<std::vector<bool>> Occupied;
    std::vector<sf::Vector2i> FreeTiles;
};

#endif // RECTANGULARGRID_HPP
