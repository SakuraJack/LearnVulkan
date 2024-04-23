Vulkan_SDK = os.getenv("VULKAN_SDK")

Dependencies = {
    Vulkan = {
        LibName = "vulkan-1",
        IncludeDir = "%{Vulkan_SDK}/Include/",
        LibDir = "%{Vulkan_SDK}/Lib/",
    },

    GLFW = {
        LibName = "GLFW",
        IncludeDir = "D:/dev/Bust/Bust/vendor/GLFW/include",
    },
}

function firstToUpper(str)
    return (str:gsub("^%l", string.upper))
end

function IncludeDependencies()
    includedirs {
        "%{Dependencies.GLFW.IncludeDir}",
        "%{Dependencies.Vulkan.IncludeDir}",
    }
end

function LinkDependencies()
    filter "system:windows"
        libdirs {
            "%{Dependencies.Vulkan.LibDir}",
        }
    filter {}
end

function LinkLibraries()
    filter "system:windows"
        links {
            Dependencies.Vulkan.LibName,
            Dependencies.GLFW.LibName,
        }
    filter {}
end