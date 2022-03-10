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
	kind "SharedLib"
	language "C++"

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
		cppdialect "C++17"
		staticruntime "Off"
        runtime "Debug"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS",
			"SK_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "SK_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SK_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SK_DIST"
		optimize "On"

-- Start of the Sandbox project
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
        "%{IncludeDir.glm}"
	}

	links
	{
		"Skye"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SK_DEBUG"
        buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "SK_RELEASE"
        buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "SK_DIST"
        buildoptions "/MD"
		optimize "On"