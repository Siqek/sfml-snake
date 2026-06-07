#ifndef ENDGAMEOVERLAY_HPP
#define ENDGAMEOVERLAY_HPP

class RenderContext;

#include "states/overlays/ButtonOverlayBase.hpp"

#include "mgui/Button.hpp"

struct EndGameOverlayTypes
{
    enum class EButton
    {
        Restart,
        BackToMenu
    };
};

class EndGameOverlay final
    : public ButtonOverlayBase<EndGameOverlayTypes::EButton, 2>
{
public:
    using EButton = EndGameOverlayTypes::EButton;

    EndGameOverlay(sf::Vector2f widnowSize, const sf::Font& font);
    ~EndGameOverlay() = default;

    void SetTitle(const std::string& text);

    void Render(sf::RenderTarget& target) override;

    void FillContext(RenderContext& context) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    sf::Text Title;
};

#endif // ENDGAMEOVERLAY_HPP
