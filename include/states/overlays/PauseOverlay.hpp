#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

#include "states/overlays/ButtonOverlayBase.hpp"

class PauseOverlay
    : public ButtonOverlayBase<3>
{
public:
    PauseOverlay(const sf::Vector2f& windowSize, const sf::Font& font);
    ~PauseOverlay() = default;

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    sf::Text PauseText;
};

#endif // PAUSEOVERLAY_HPP
