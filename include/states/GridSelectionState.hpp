#ifndef GRIDSELECTIONSTATE_HPP
#define GRIDSELECTIONSTATE_HPP

#include "states/State.hpp"

#include "game/grid/Grid.hpp"

#include "mgui/ArrowSelector.hpp"

#include "mgui/Button.hpp"

class GridSelectionState final
    : public IState
{
public:
    GridSelectionState(StateContext& context);

    void Update(float dt) override;

    void Render(sf::RenderTarget& target) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

private:
    void UpdateGameSettings();

    void UpdateUIScaling();

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

#endif // GRIDSELECTIONSTATE_HPP
