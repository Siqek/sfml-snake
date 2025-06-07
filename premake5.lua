workspace "Game"
    location "build"
    configurations { "Debug", "Release" }

project "Game"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "Game"
    objdir "build/obj/%{cfg.buildcfg}"

    includedirs { "include" }

    files { "include/**.hpp", "src/**.cpp" }

    pchheader "include/stdafx.hpp"
    pchsource "src/stdafx.cpp"

    filter "system:linux"
        links {
            "X11",
            "sfml-graphics",
            "sfml-window",
            "sfml-system"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"
        sanitize { "Address", "UndefinedBehavior" }
        warnings "Extra"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
