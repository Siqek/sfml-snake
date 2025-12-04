#ifndef GRIDSELECTIONOVERLAY_HPP
#define GRIDSELECTIONOVERLAY_HPP

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

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Update(const sf::RenderWindow& window) override;
    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    void UpdateGridImitation(EGridType gridType);

    void AdjustHoleSizeToGridSize();

    sf::RectangleShape GridSelectionBackground;

    mgui::ArrowSelector<EGridType> GridSelector;
    static std::vector<mgui::ArrowSelector<EGridType>::Option> GridSelectorOptions;

    mgui::ArrowSelector<sf::Vector2<uint8_t>> GridSizeSelector;

    mgui::ArrowSelector<sf::Vector2i> GridHoleSizeSelector;
    static std::vector<mgui::ArrowSelector<sf::Vector2i>::Option> GridHoleSizeSelectorOptions;

    sf::Text GridSizeSelectorLabel;
    sf::Text GridHoleSizeSelectorLabel;

    sf::RectangleShape GridImitation;

    mgui::Button PlayButton;
};

#endif // GRIDSELECTIONOVERLAY_HPP
