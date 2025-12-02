#ifndef COLORS_HPP
#define COLORS_HPP

#include <cstdint>

namespace Colors::Hex {
    static constexpr std::uint32_t Background          = 0x0F0E17FF;

    static constexpr std::uint32_t OverlayBackground   = 0x000000A0;


    static constexpr std::uint32_t TextMain            = 0xE9FFE0FF;
    static constexpr std::uint32_t TextMainOutline     = 0x0B0B10FF;

    static constexpr std::uint32_t TextAlt             = 0xF5F5FAFF;
    static constexpr std::uint32_t TextAltOutline      = 0x2B2633FF;


    static constexpr std::uint32_t SnakeBody           = 0xCCFFBDFF;
    static constexpr std::uint32_t SnakeOutline        = 0x7ECA9CFF;


    static constexpr std::uint32_t BoardCellPrimary    = 0x40394AFF;
    static constexpr std::uint32_t BoardCellSecondary  = 0x1C1427FF;


    static constexpr std::uint32_t ButtonIdleBg        = 0x18151CFF;
    static constexpr std::uint32_t ButtonIdleOutline   = 0x3D6B5BFF;

    static constexpr std::uint32_t ButtonHoverBg       = 0x0F0E17FF;
    static constexpr std::uint32_t ButtonHoverOutline  = 0x7ECA9CFF;

    static constexpr std::uint32_t ButtonActiveBg      = 0x1E3B33FF;
    static constexpr std::uint32_t ButtonActiveOutline = 0xCCFFBDFF;


    static constexpr std::uint32_t GridSelectionCardBackground  = 0x1A1A22FF;
    static constexpr std::uint32_t GridSelectionCardOutline     = 0x4A7B68FF;

    static constexpr std::uint32_t GridImitationColor           = 0x3C3547FF;

}

#endif // COLORS_HPP
