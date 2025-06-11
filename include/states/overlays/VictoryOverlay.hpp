#ifndef VICTORYOVERLAY_HPP
#define VICTORYOVERLAY_HPP

#include "states/overlays/Overlay.hpp"

#include "mgui/Button.hpp"

class VictoryOverlay
    : public Overlay
{
public:
    VictoryOverlay(const sf::Vector2f& windowSize, const sf::Font& font);
    ~VictoryOverlay() = default;

    bool isBackToMenuButtonReleased() { return backToMenuButton.isReleased(); }

    void update(const sf::RenderWindow& window) override;
    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(const sf::Vector2f& newWindowSize) override;

    sf::Text victoryText;

    mgui::Button backToMenuButton;
};

#endif // VICTORYOVERLAY_HPP
