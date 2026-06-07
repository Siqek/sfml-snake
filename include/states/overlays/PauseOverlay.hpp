#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

class RenderContext;

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

class PauseOverlay final
    : public ButtonOverlayBase<PauseOverlayTypes::EButton, 3>
{
public:
    using EButton = PauseOverlayTypes::EButton;

    PauseOverlay(sf::Vector2f windowSize, const sf::Font& font);
    ~PauseOverlay() = default;

    void Render(sf::RenderTarget& target) override;

    void FillContext(RenderContext& context) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    sf::Text PauseText;
};

#endif // PAUSEOVERLAY_HPP
