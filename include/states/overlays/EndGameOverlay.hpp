#ifndef ENDGAMEOVERLAY_HPP
#define ENDGAMEOVERLAY_HPP

#include "states/overlays/Overlay.hpp"

#include "mgui/Button.hpp"

class EndGameOverlay
    : public Overlay
{
public:
    EndGameOverlay(sf::Vector2f widnowSize, const sf::Font& font);
    ~EndGameOverlay() = default;

    bool isRestartButtonReleased() const { return restartButton.isReleased(); }
    bool isBackToMenuButtonReleased() const { return backToMenuButton.isReleased(); }

    void setTitle(const std::string& text);

    void update(const sf::RenderWindow& window) override;
    void render(sf::RenderTarget& target) override;

private:
    void updateUIScaling(sf::Vector2f newWindowSize) override;

    sf::Text title;

    mgui::Button restartButton;
    mgui::Button backToMenuButton;
};

#endif // ENDGAMEOVERLAY_HPP
