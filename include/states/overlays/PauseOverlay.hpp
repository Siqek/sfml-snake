#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

#include "states/overlays/ButtonOverlayBase.hpp"

struct PauseOverlayTypes
{
    enum class EButton
    {
        Continue,
        Restart,
        BackToMenu
    };
};

class PauseOverlay
    : public ButtonOverlayBase<PauseOverlayTypes::EButton, 3>
{
public:
    using EButton = PauseOverlayTypes::EButton;

    PauseOverlay(const sf::Vector2f& windowSize, const sf::Font& font);
    ~PauseOverlay() = default;

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    sf::Text PauseText;
};

#endif // PAUSEOVERLAY_HPP
