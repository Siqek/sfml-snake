#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

#include "states/overlays/ButtonOverlayBase.hpp"

class PauseOverlay
    : public ButtonOverlayBase<3>
{
public:
    PauseOverlay(const sf::Vector2f& windowSize, const sf::Font& font);
    ~PauseOverlay() = default;

    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(sf::Vector2f newWindowSize) override;

    sf::Text pauseText;
};

#endif // PAUSEOVERLAY_HPP
