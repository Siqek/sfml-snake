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

    void setTitle(const std::string& text);

    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(sf::Vector2f newWindowSize) override;

    sf::Text title;
};

#endif // ENDGAMEOVERLAY_HPP
