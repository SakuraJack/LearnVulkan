include "Dependencies.lua"

workspace "VulkanPractice"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    startproject "VulkanPractice"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Dependencies"
        include "D:/dev/Bust/Bust/vendor/GLFW"
    group ""

    project "VulkanPractice"
    kind "ConsoleApp"
    language "C++"
        cppdialect "C++17"
        targetdir "bin/%{cfg.buildcfg}"
        objdir "bin/obj/%{cfg.buildcfg}"
        pchheader "pch.h"
        pchsource "src/pch.cpp"
        files { 
            "src/**.h", 
            "src/**.cpp" 
        }

        includedirs {
            "src/", "vendor/"
        }

        IncludeDependencies()
        LinkDependencies()
        LinkLibraries()

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"
            optimize "Off"
        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"