#ifndef APPLE_HPP
#define APPLE_HPP

class RenderContext;

class Apple
{
public:
    Apple();
    ~Apple() = default;

    sf::Vector2i GetPosition() const { return Position; };

    void SetTileSize(float tileSize);

    bool Spawn(const std::vector<sf::Vector2i>& freeTiles);
    void Render(sf::RenderTarget& target, float offsetX = 0, float offsetY = 0);

    void FillContext(RenderContext& context, const sf::Vector2f& offset);

private:
    sf::Vector2i Position;
    sf::RectangleShape AppleShape;

    float TileSize;

    bool bIsSpawned;
};

#endif // APPLE_HPP