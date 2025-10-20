#ifndef GRID_HPP
#define GRID_HPP

class Grid
{
public:
    Grid(const sf::Vector2i& size);
    virtual ~Grid() = default;

    virtual bool isWithinBoundaries(const sf::Vector2i& position) const;

    bool isOccupied(const sf::Vector2i& position) const { return occupied[position.x][position.y]; }

    const sf::Vector2i& getSize() const { return size; }

    const std::vector<sf::Vector2i>& getFreeTiles() const { return freeTiles; }

    virtual size_t getTotalTileCount() const { return static_cast<size_t>(size.x * size.y); }

    virtual bool freeTile(const sf::Vector2i& position);
    virtual bool occupyTile(const sf::Vector2i& position);

    static constexpr int MinGridSize = 4;

private:
    void removeFromFreeTiles(const sf::Vector2i& position);

    sf::Vector2i size;

    std::vector<std::vector<bool>> occupied;
    std::vector<sf::Vector2i> freeTiles;
};

#endif // GRID_HPP
