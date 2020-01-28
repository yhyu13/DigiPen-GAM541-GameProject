workspace "game-project"
	architecture "x86_64"
	startproject "application"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"]     = "engine/vendors/glfw/include"
IncludeDir["glad"]     = "engine/vendors/glad/include"
IncludeDir["stbi"]     = "engine/vendors/stbi"
IncludeDir["glm"]      = "engine/vendors/glm"
IncludeDir["jsoncpp"]  = "engine/vendors/jsoncpp/include"

project "engine"
	location "engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp",

		"%{prj.name}/vendors/glad/src/**.c",

		"%{prj.name}/vendors/stbi/**.h",
		"%{prj.name}/vendors/stbi/**.cpp",

		"%{prj.name}/vendors/glm/glm/**.hpp",
		"%{prj.name}/vendors/glm/glm/**.inl",

		"%{prj.name}/vendors/jsoncpp/include/json/**.h",
		"%{prj.name}/vendors/jsoncpp/src/lib_json/**.h",
		"%{prj.name}/vendors/jsoncpp/src/lib_json/**.cpp",
		"%{prj.name}/vendors/jsoncpp/src/lib_json/**.inl"
	}

	includedirs
	{
		"%{prj.name}/source",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stbi}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.jsoncpp}"
	}

	libdirs
	{
		"%{prj.name}/vendors/glfw"
	}

	links
	{
		"glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/application/\"")
		}

	filter "configurations:Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MD"
		optimize "On"

project "application"
	location "application"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"engine/source",
		"engine/vendors",
		"%{IncludeDir.glm}"
	}

	links
	{
		"engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{

		}

	filter "configurations:Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MD"
		optimize "On"
