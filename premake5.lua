workspace "Game"
    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "Game"

project "Game"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "Game"
    objdir "build/obj/%{cfg.buildcfg}"

    -- Precompiled headers
    pchheader "stdafx.hpp"
    pchsource "src/stdafx.cpp"

    -- Include all source and header files
    files { "include/**.hpp", "src/**.cpp" }
    includedirs { "include" }

    -- Windows-specific includes and libraries (SFML)
    filter "system:windows"
        includedirs { "external/SFML/include" }
        libdirs { "external/SFML/lib" }

    -- Force the use of main() instead of WinMain() in Visual Studio
    filter { "system:windows", "action:vs*" }
        linkoptions { "/ENTRY:mainCRTStartup" }

    -- Link SFML libraries for Release on Windows
    filter { "system:windows", "configurations:Release" }
        links {
            "sfml-graphics",
            "sfml-window",
            "sfml-system"
        }

    -- Link SFML debug libraries on Windows
    filter { "system:windows", "configurations:Debug" }
        links {
            "sfml-graphics-d",
            "sfml-window-d",
            "sfml-system-d"
        }

    -- Linux-specific linking
    filter "system:linux"
        links {
            "X11",
            "sfml-graphics",
            "sfml-window",
            "sfml-system"
        }

    -- Debug settings
    filter "configurations:Debug"
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"
        warnings "Extra"

    -- Additional Linux-specific debug settings
    filter { "system:linux", "configurations:Debug" }
        sanitize { "Address", "UndefinedBehavior" }

    -- Release settings
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    -- Copy SFML DLLs after build (Debug on Windows)
    filter { "system:windows", "configurations:Debug", "action:vs*" }
        postbuildcommands {
            '{COPY} "external/SFML/bin/openal32.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-graphics-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-window-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-system-d-2.dll" "%{cfg.targetdir}"',
        }

    filter { "system:windows", "configurations:Debug", "action:gmake" }
        postbuildcommands {
            'cp -u "external/SFML/bin/openal32.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-graphics-d-2.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-window-d-2.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-system-d-2.dll" "%{cfg.targetdir}"',
        }

    -- Copy SFML DLLs after build (Release on Windows)
    filter { "system:windows", "configurations:Release", "action:vs*" }
        postbuildcommands {
            '{COPY} "external/SFML/bin/openal32.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-graphics-2.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-window-2.dll" "%{cfg.targetdir}"',
            '{COPY} "external/SFML/bin/sfml-system-2.dll" "%{cfg.targetdir}"'
        }

    filter { "system:windows", "configurations:Release", "action:gmake" }
        postbuildcommands {
            'cp -u "external/SFML/bin/openal32.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-graphics-2.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-window-2.dll" "%{cfg.targetdir}"',
            'cp -u "external/SFML/bin/sfml-system-2.dll" "%{cfg.targetdir}"'
        }
