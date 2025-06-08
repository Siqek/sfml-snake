#ifndef GAMEOVEROVERLAY_HPP
#define GAMEOVEROVERLAY_HPP

#include "states/overlays/Overlay.hpp"

class GameOverOverlay
    : public Overlay
{
public:
    GameOverOverlay(const sf::Vector2f& windowSize);
    ~GameOverOverlay() = default;

    void update() override;
    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(const sf::Vector2f& newWindowSize);
};

#endif // GAMEOVEROVERLAY_HPP
