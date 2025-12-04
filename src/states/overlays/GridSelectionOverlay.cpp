#include "stdafx.hpp"
#include "states/overlays/GridSelectionOverlay.hpp"

#include "config/GameSettingsOptions.hpp"

#include "config/Colors.hpp"

std::vector<mgui::ArrowSelector<GridSelectionOverlay::EGridType>::Option> GridSelectionOverlay::GridSelectorOptions = {
    { "rectangular", "Rectangular Grid", EGridType::Rectangular },
    { "donut", "Donut Grid", EGridType::Donut }
};

std::vector<mgui::ArrowSelector<sf::Vector2i>::Option> GridSelectionOverlay::GridHoleSizeSelectorOptions = {
    { "2x2", "2x2", { 2, 2 } },
    { "4x4", "4x4", { 4, 4 } },
    { "6x6", "6x6", { 6, 6 } },
    { "8x8", "8x8", { 8, 8 } },
    { "10x10", "10x10", { 10, 10 } }
};

GridSelectionOverlay::GridSelectionOverlay(const sf::Vector2f& windowSize, const sf::Font& font)
    : Overlay(windowSize, sf::Color::Transparent),
    GridSelector(GridSelectorOptions, font),
    GridSizeSelector(GameSettingsOptions::GridSizeOptions, font),
    GridHoleSizeSelector(GridHoleSizeSelectorOptions, font),
    GridSizeSelectorLabel("Grid Size", font),
    GridHoleSizeSelectorLabel("Hole Size", font)
{
    GridSelectionBackground.setFillColor(sf::Color(Colors::Hex::GridSelectionCardBackground));
    GridSelectionBackground.setOutlineColor(sf::Color(Colors::Hex::GridSelectionCardOutline));

    GridSizeSelectorLabel.setFillColor(sf::Color(Colors::Hex::TextMain));
    GridSizeSelectorLabel.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    GridHoleSizeSelectorLabel.setFillColor(sf::Color(Colors::Hex::TextMain));
    GridHoleSizeSelectorLabel.setOutlineColor(sf::Color(Colors::Hex::TextMainOutline));

    GridImitation.setFillColor(sf::Color(Colors::Hex::GridImitationColor));
    GridImitation.setOutlineColor(sf::Color(Colors::Hex::GridImitationColor));

    UpdateGridImitation(EGridType::Rectangular);

    const auto styleElement = [](auto& elem) -> void {
        elem.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        elem.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        elem.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        elem.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        elem.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        elem.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    };

    styleElement(GridSelector);
    styleElement(GridSizeSelector);
    styleElement(GridHoleSizeSelector);

    PlayButton.setFont(font);
    PlayButton.setText("Play");

    styleElement(PlayButton);

    UpdateUIScaling(windowSize);
}

void GridSelectionOverlay::OnWindowResize(const sf::Vector2f& windowSize)
{
    Overlay::OnWindowResize(windowSize);
    UpdateUIScaling(windowSize);
}

void GridSelectionOverlay::Update(const sf::RenderWindow& window)
{
    GridSelector.update(window);
    GridSizeSelector.update(window);
    GridHoleSizeSelector.update(window);

    PlayButton.update(window);

    if (GridSelector.hasActiveOptionChanged())
    {
        UpdateGridImitation(GridSelector.getActiveValue());

        if (GridSelector.getActiveValue() == EGridType::Donut)
        {
            AdjustHoleSizeToGridSize();
        }
    }

    if (GridSizeSelector.hasActiveOptionChanged())
    {
        if (GridSelector.getActiveValue() == EGridType::Donut)
        {
            AdjustHoleSizeToGridSize();
        }
    }

    if (GridHoleSizeSelector.hasActiveOptionChanged())
    {
        AdjustHoleSizeToGridSize();
    }
}

void GridSelectionOverlay::Render(sf::RenderTarget& target)
{
    if (!GetIsActive())
    {
        return;
    }

    target.draw(GridSelectionBackground);

    target.draw(GridImitation);

    GridSelector.render(target);

    GridSizeSelector.render(target);
    target.draw(GridSizeSelectorLabel);

    if (GridSelector.getActiveValue() == EGridType::Donut)
    {
        GridHoleSizeSelector.render(target);
        target.draw(GridHoleSizeSelectorLabel);
    }

    PlayButton.render(target);
}

void GridSelectionOverlay::UpdateUIScaling(sf::Vector2f windowSize)
{
    const sf::Vector2f backgroundSize = sf::Vector2f(windowSize.x * 0.3f, windowSize.y * 0.75f);
    const float backgroundYTopMargin = windowSize.y * 0.05f;
    const sf::Vector2f backgroundPosition = sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f - (windowSize.y - backgroundSize.y) / 2.f + backgroundYTopMargin);
    const float outlineThickness = backgroundSize.x / 128.f;

    GridSelectionBackground.setPosition(backgroundPosition);
    GridSelectionBackground.setSize(backgroundSize);
    GridSelectionBackground.setOrigin(backgroundSize / 2.f);
    GridSelectionBackground.setOutlineThickness(outlineThickness);

    const float gridImitationSideSize = std::min(backgroundSize.x * 0.8f, backgroundSize.y * 0.4f);
    const sf::Vector2f gridImitationSize = sf::Vector2f(gridImitationSideSize, gridImitationSideSize);

    const float gridImitationYMargin = backgroundSize.y / 10.f;

    GridImitation.setPosition(sf::Vector2f(backgroundPosition.x, backgroundPosition.y - backgroundSize.y / 2.f + gridImitationSideSize / 2.f + gridImitationYMargin));
    GridImitation.setSize(gridImitationSize);
    GridImitation.setOrigin(gridImitationSize / 2.f);
    GridImitation.setOutlineThickness(gridImitationSideSize * -0.33f);

    const sf::Vector2f selectorSize = sf::Vector2f(backgroundSize.x * 0.9f, backgroundSize.y * 0.064f);
    const float selectorOutlineThickness = selectorSize.y / 16.f;

    GridSelector.setPosition(sf::Vector2f(backgroundPosition.x, backgroundPosition.y + backgroundSize.y / 2.f - selectorSize.y));
    GridSelector.setSize(selectorSize);
    GridSelector.setOrigin(selectorSize / 2.f);
    GridSelector.setOutlineThickness(selectorOutlineThickness);

    const sf::Vector2f subselectorSize = selectorSize * 0.8f;
    const float subselectorOutlineThickness = selectorOutlineThickness * 0.8f;

    const unsigned subselectorLabelCharacterSize = static_cast<unsigned>(subselectorSize.y * 0.6f);
    const float subselectorLabelOutlineThickness = static_cast<float>(subselectorLabelCharacterSize / 32.f);

    const float subselectorYGapToLabel = subselectorSize.y * 1.1f;
    const float yGapBetweenSubselectors = subselectorSize.y * 1.2f;
    const float subselectorYMargin = backgroundSize.y / 20.f;

    const sf::Vector2f gridSizeSelectorLabelPosition = backgroundPosition + sf::Vector2f(0, subselectorSize.y / 2.f + subselectorYMargin + subselectorYGapToLabel);
    const sf::Vector2f gridSizeSelectorPosition = sf::Vector2f(gridSizeSelectorLabelPosition.x, gridSizeSelectorLabelPosition.y + subselectorYGapToLabel);
    const sf::Vector2f gridHoleSizeSelectorLabelPosition = sf::Vector2f(gridSizeSelectorPosition.x, gridSizeSelectorPosition.y + yGapBetweenSubselectors);
    const sf::Vector2f gridHoleSizeSelectorPosition = sf::Vector2f(gridHoleSizeSelectorLabelPosition.x, gridHoleSizeSelectorLabelPosition.y + subselectorYGapToLabel);

    const auto centerOrigin = [](sf::Text& label) -> void {
        const auto labelLocalBounds = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(labelLocalBounds.left + labelLocalBounds.width / 2.f, labelLocalBounds.top + labelLocalBounds.height / 2.f));
    };

    GridSizeSelectorLabel.setPosition(gridSizeSelectorLabelPosition);
    GridSizeSelectorLabel.setCharacterSize(subselectorLabelCharacterSize);
    GridSizeSelectorLabel.setOutlineThickness(subselectorLabelOutlineThickness);
    centerOrigin(GridSizeSelectorLabel);

    GridSizeSelector.setPosition(gridSizeSelectorPosition);
    GridSizeSelector.setSize(subselectorSize);
    GridSizeSelector.setOrigin(subselectorSize / 2.f);
    GridSizeSelector.setOutlineThickness(subselectorOutlineThickness);

    GridHoleSizeSelectorLabel.setPosition(gridHoleSizeSelectorLabelPosition);
    GridHoleSizeSelectorLabel.setCharacterSize(subselectorLabelCharacterSize);
    GridHoleSizeSelectorLabel.setOutlineThickness(subselectorLabelOutlineThickness);
    centerOrigin(GridHoleSizeSelectorLabel);

    GridHoleSizeSelector.setPosition(gridHoleSizeSelectorPosition);
    GridHoleSizeSelector.setSize(subselectorSize);
    GridHoleSizeSelector.setOrigin(subselectorSize / 2.f);
    GridHoleSizeSelector.setOutlineThickness(subselectorOutlineThickness);

    const float freeSpaceUnderBackground = windowSize.y - backgroundYTopMargin - backgroundSize.y;
    const sf::Vector2f playButtonPosition = sf::Vector2f(windowSize.x / 2.f, backgroundYTopMargin + backgroundSize.y + freeSpaceUnderBackground / 2.f);
    const unsigned playButtonCharacterSize = static_cast<unsigned>(freeSpaceUnderBackground / 6.f);

    PlayButton.setPosition(playButtonPosition);
    PlayButton.setCharacterSize(playButtonCharacterSize);

    const sf::Vector2f playButtonTextSize = PlayButton.geTextLocalBounds().getSize();
    const sf::Vector2f playButtonSize = sf::Vector2f(playButtonTextSize.x * 6.f, playButtonTextSize.y * 2.f);
    const float playButtonOutlineThickness = playButtonSize.y / 16.f;

    PlayButton.setSize(playButtonSize);
    PlayButton.setOrigin(playButtonSize / 2.f);
    PlayButton.setOutlineThickness(playButtonOutlineThickness);
}

void GridSelectionOverlay::UpdateGridImitation(EGridType gridType)
{
    switch (gridType)
    {
        case EGridType::Rectangular:
            GridImitation.setFillColor(GridImitation.getOutlineColor());
            GridImitation.setOutlineColor(sf::Color::Transparent);
            break;

        case EGridType::Donut:
            GridImitation.setOutlineColor(GridImitation.getFillColor());
            GridImitation.setFillColor(sf::Color::Transparent);
            break;

        default:
            break;
    }
}

void GridSelectionOverlay::AdjustHoleSizeToGridSize()
{
    const sf::Vector2i activeHoleSize = GridHoleSizeSelector.getActiveValue();
    const sf::Vector2i activeGridSize = sf::Vector2i(GridSizeSelector.getActiveValue());

    if (activeHoleSize.x >= activeGridSize.x || activeHoleSize.y >= activeGridSize.y)
    {
        std::string holeSizeId = GridHoleSizeSelectorOptions.at(0).id;

        for (const auto& holeSizeOption : GridHoleSizeSelectorOptions)
        {
            if (holeSizeOption.value.x >= activeGridSize.x || holeSizeOption.value.y >= activeGridSize.y)
            {
                break;
            }
            holeSizeId = holeSizeOption.id;
        }

        GridHoleSizeSelector.setActiveOption(holeSizeId);
    }
}
