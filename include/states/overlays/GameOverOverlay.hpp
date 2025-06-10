#ifndef GAMEOVEROVERLAY_HPP
#define GAMEOVEROVERLAY_HPP

#include "states/overlays/Overlay.hpp"

#include "mgui/Button.hpp"

class GameOverOverlay
    : public Overlay
{
public:
    GameOverOverlay(const sf::Vector2f& windowSize, const sf::Font& font);
    ~GameOverOverlay() = default;

    bool isBackToMenuButtonReleased() { return backToMenuButton.isReleased(); }

    void update(const sf::RenderWindow& window) override;
    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(const sf::Vector2f& newWindowSize);

    sf::Text gameOverText;

    mgui::Button backToMenuButton;
};

#endif // GAMEOVEROVERLAY_HPP
