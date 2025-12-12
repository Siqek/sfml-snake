#ifndef GRIDSELECTIONOVERLAY_HPP
#define GRIDSELECTIONOVERLAY_HPP

class GameSettings;

#include "snake/grid/Grid.hpp"

#include "states/overlays/Overlay.hpp"

#include "mgui/ArrowSelector.hpp"

#include "mgui/Button.hpp"

class GridSelectionOverlay
    : public Overlay
{
public:
    GridSelectionOverlay(const sf::Vector2f& windowSize, const sf::Font& font);

    bool IsPlayButtonReleased() const;

    void UpdateGameSettings(GameSettings& outGameSettings);

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Update(const sf::RenderWindow& window) override;
    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    void UpdateGridImitation(EGridType gridType);

    void AdjustHoleSizeToGridSize();

    sf::RectangleShape GridSelectionBackground;

    mgui::ArrowSelector<EGridType> GridTypeSelector;
    mgui::ArrowSelector<sf::Vector2i> GridSizeSelector;
    mgui::ArrowSelector<sf::Vector2i> GridHoleSizeSelector;

    sf::Text GridSizeSelectorLabel;
    sf::Text GridHoleSizeSelectorLabel;

    sf::RectangleShape GridImitation;

    mgui::Button PlayButton;
};

#endif // GRIDSELECTIONOVERLAY_HPP
