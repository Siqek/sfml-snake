#ifndef OVERLAY_HPP
#define OVERLAY_HPP

class Overlay
{
public:
    Overlay(const sf::Vector2f& windowSize, sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 128u));
    virtual ~Overlay() = default;

    void setBackgroundColor(sf::Color color) { windowOverlay.setFillColor(color); }

    void close() { isActive = false; }
    void show() { isActive = true; }

    bool getIsActive() const { return isActive; }

    void onWindowResize(const sf::Vector2f& newWindowSize);

    virtual void update(const sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderTarget& target) = 0;

protected:
    void renderWindowOverlay(sf::RenderTarget& target) { target.draw(windowOverlay); }

    sf::RectangleShape windowOverlay;

private:
    virtual void updateUIScaling(sf::Vector2f newWindowSize) = 0;

    bool isActive;
};

#endif // OVERLAY_HPP
