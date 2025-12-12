#ifndef GAMESETTINGSOPTIONS_HPP
#define GAMESETTINGSOPTIONS_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>

#include "snake/grid/Grid.hpp"

#include "mgui/ArrowSelector.hpp"

namespace GameSettingsOptions {
    inline const std::vector<mgui::ArrowSelector<EGridType>::Option> GridTypeOptions = {
        { "rectangular", "Rectangular Grid", EGridType::Rectangular },
        { "rectangular_donut", "Donut Grid", EGridType::RectangularDonut }
    };

    inline const std::vector<mgui::ArrowSelector<sf::Vector2i>::Option> GridSizeOptions = {
        { "8x8",   "8x8",   { 8, 8 } },
        { "10x10", "10x10", { 10, 10 } },
        { "12x12", "12x12", { 12, 12 } },
        { "16x16", "16x16", { 16, 16 } },
        { "20x20", "20x20", { 20, 20 } }
    };

    inline const std::vector<mgui::ArrowSelector<sf::Vector2i>::Option> GridHoleSizeOptions = {
        { "2x2", "2x2", { 2, 2 } },
        { "4x4", "4x4", { 4, 4 } },
        { "6x6", "6x6", { 6, 6 } },
        { "8x8", "8x8", { 8, 8 } },
        { "10x10", "10x10", { 10, 10 } }
    };

    inline const std::vector<mgui::ArrowSelector<float>::Option> SnakeSpeedOptions = {
        { "slow",   "slow",   2.f },
        { "normal", "normal", 4.f },
        { "fast",   "fast",   6.f },
        { "extremely_fast", "extremely fast", 8.f }
    };

    inline const std::vector<mgui::ArrowSelector<unsigned>::Option> MaxAppleCountOptions = {
        { "1",  "1",  1u },
        { "2",  "2",  2u },
        { "3",  "3",  3u },
        { "5",  "5",  5u },
        { "10", "10", 10u }
    };

    static constexpr size_t DefaultGridTypeOptionIndex      = 0;
    static constexpr size_t DefaultGridSizeOptionIndex      = 1;
    static constexpr size_t DefaultGridHoleSizeOptionIndex  = 1;
    static constexpr size_t DefaultSnakeSpeedOptionIndex    = 1;
    static constexpr size_t DefaultMaxAppleCountOptionIndex = 0;

} // GameSettingsOptions

#endif // GAMESETTINGSOPTIONS_HPP
