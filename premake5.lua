workspace "Skye"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Skye/vendor/GLFW/include"
IncludeDir["Glad"] = "Skye/vendor/Glad/include"
IncludeDir["ImGui"] = "Skye/vendor/imgui"
IncludeDir["glm"] = "Skye/vendor/glm"

-- Include the GLFW and Glad premake projects inside this premake file!
include "Skye/vendor/GLFW"
include "Skye/vendor/Glad"
include "Skye/vendor/imgui"

-- Start of the Skye project
project "Skye"
	location "Skye"
	kind "StaticLib"
	language "C++"
    cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "skpch.hpp"
	pchsource "Skye/src/skpch.cpp"

	files
	{
		"%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
	}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
	}

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS",
		    "SK_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "SK_DEBUG"
        runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SK_RELEASE"
        runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SK_DIST"
        runtime "Release"
		optimize "on"

-- Start of the Sandbox project
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Skye/vendor/spdlog/include",
		"Skye/src",
        "Skye/vendor",
        "%{IncludeDir.glm}"
	}

	links
	{
		"Skye"
	}

	filter "system:windows"
        staticruntime "off"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SK_DEBUG"
        runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SK_RELEASE"
        runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SK_DIST"
        runtime "Release"
		optimize "on"