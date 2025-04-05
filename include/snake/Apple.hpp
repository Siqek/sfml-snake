#ifndef APPLE_HPP
#define APPLE_HPP

class Apple
{
public:
    Apple();
    ~Apple() = default;

    sf::Vector2i getPosition() const;

    void setTileSize(float tileSize);

    void spawn(std::vector<sf::Vector2i> freeTiles);
    void render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

private:
    sf::Vector2i position;
    sf::RectangleShape appleShape;

    float tileSize;
};

#endif // APPLE_HPP