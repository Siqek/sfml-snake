#ifndef OVERLAY_HPP
#define OVERLAY_HPP

class Overlay
{
public:
    Overlay(const sf::Vector2f& windowSize, sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 128u));
    virtual ~Overlay() = default;

    void SetBackgroundColor(sf::Color color);

    void Close();
    void Show();

    bool GetIsActive() const { return bIsActive; }

    virtual void OnWindowResize(const sf::Vector2f& windowSize);

    virtual void Update(const sf::RenderWindow& window);
    virtual void Render(sf::RenderTarget& target);

private:
    sf::RectangleShape WindowOverlay;

    bool bIsActive;
};

#endif // OVERLAY_HPP
