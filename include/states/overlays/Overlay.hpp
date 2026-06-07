#ifndef OVERLAY_HPP
#define OVERLAY_HPP

class RenderContext;

class Overlay
{
public:
    Overlay(sf::Vector2f windowSize, sf::Color backgroundColor = sf::Color(0u, 0u, 0u, 128u));
    virtual ~Overlay() = default;

    void SetBackgroundColor(sf::Color color);

    void Close();
    void Show();

    bool IsActive() const { return bIsActive; }

    virtual void Render(sf::RenderTarget& target);

    virtual void FillContext(RenderContext& context);

    virtual void OnWindowResize(const sf::Event::SizeEvent& size);

private:
    sf::RectangleShape WindowOverlay;

    bool bIsActive;
};

#endif // OVERLAY_HPP
