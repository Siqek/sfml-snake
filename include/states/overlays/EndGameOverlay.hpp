#ifndef ENDGAMEOVERLAY_HPP
#define ENDGAMEOVERLAY_HPP

#include "states/overlays/ButtonOverlayBase.hpp"

#include "mgui/Button.hpp"

class EndGameOverlay
    : public ButtonOverlayBase<2>
{
public:
    EndGameOverlay(sf::Vector2f widnowSize, const sf::Font& font);
    ~EndGameOverlay() = default;

    void SetTitle(const std::string& text);

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    sf::Text Title;
};

#endif // ENDGAMEOVERLAY_HPP
