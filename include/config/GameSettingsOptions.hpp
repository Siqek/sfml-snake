#ifndef GAMESETTINGSOPTIONS_HPP
#define GAMESETTINGSOPTIONS_HPP

#include "stdafx.hpp"

#include "mgui/ArrowSelector.hpp"

namespace GameSettingsOptions {
    inline const std::vector<mgui::ArrowSelector<sf::Vector2<uint8_t>>::Option> GridSizeOptions = {
        { "8x8",   "8x8",   { 8, 8 } },
        { "10x10", "10x10", { 10, 10 } },
        { "12x12", "12x12", { 12, 12 } },
        { "20x20", "20x20", { 20, 20 } }
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


    static constexpr size_t DefaultGridSizeOptionIndex      = 1;
    static constexpr size_t DefaultSnakeSpeedOptionIndex    = 1;
    static constexpr size_t DefaultMaxAppleCountOptionIndex = 0;

} // GameSettingsOptions

#endif // GAMESETTINGSOPTIONS_HPP
