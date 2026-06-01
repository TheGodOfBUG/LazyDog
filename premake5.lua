workspace "LazyDog"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "LazyDog/vendor/GLFW/include"
IncludeDir["GLAD"] = "LazyDog/vendor/GLAD/include"
IncludeDir["ImGui"] = "LazyDog/vendor/imgui"
IncludeDir["glm"] = "LazyDog/vendor/glm"
IncludeDir["stb_image"] = "LazyDog/vendor/stb_image"

include "LazyDog/vendor/GLFW"
include "LazyDog/vendor/GLAD"
include "LazyDog/vendor/imgui"

project "LazyDog"
    location "LazyDog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
   
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ldpch.h"
    pchsource "%{prj.name}/src/ldpch.cpp"

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "LD_PLATFORM_WINDOWS",
            "LD_ENABLE_ASSERTS",
            "LD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "LD_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "LD_RELEASE"
        buildoptions "/MD"
        optimize "On"
    
    filter "configurations:Dist"
        defines "LD_DIST"
        buildoptions "/MD"
        optimize "On"
    
    filter {"system:windows", "configurations:Debug"}
        buildoptions "/utf-8"
        
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
    }

    includedirs 
    {
        "LazyDog/vendor/spdlog/include",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "LazyDog/src"
    }

    links 
    {
        "LazyDog"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "LD_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "LD_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "LD_RELEASE"
        buildoptions "/MD"
        optimize "On"
    
    filter "configurations:Dist"
        defines "LD_DIST"
        buildoptions "/MD"
        optimize "On"
    
    filter {"system:windows", "configurations:Debug"}
        buildoptions "/utf-8"