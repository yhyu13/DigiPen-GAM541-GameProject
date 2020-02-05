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
IncludeDir["fmod_core"]  = "engine/vendors/fmod/api/core/inc"
IncludeDir["fmod_bank"]  = "engine/vendors/fmod/api/fsbank/inc"
IncludeDir["fmod_studio"]  = "engine/vendors/fmod/api/studio/inc"

LibDir = {}
LibDir["glfw"] = "engine/vendors/glfw"
LibDir["fmod_core"] = "engine/vendors/fmod/api/core/lib/x64"
LibDir["fmod_bank"] = "engine/vendors/fmod/api/fsbank/lib/x64"
LibDir["fmod_studio"] = "engine/vendors/fmod/api/studio/lib/x64"

LibName = {}
LibName["fmod_core"] = "fmod_vc.lib"
LibName["fmod_bank"] = "fsbank_vc.lib"
LibName["fmod_studio"] = "fmodstudio_vc.lib"

DllName = {}
DllName["fmod_core"] = "fmod.dll"
DllName["fmod_bank"] = "fsbank.dll"
DllName["fmod_bank2"] = "libfsbvorbis64.dll"
DllName["fmod_studio"] = "fmodstudio.dll"

include "engine/vendors/glfw"

project "engine"
	location "engine"
	kind "StaticLib"
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
		"%{IncludeDir.jsoncpp}",
		"%{IncludeDir.fmod_core}",
		"%{IncludeDir.fmod_bank}",
		"%{IncludeDir.fmod_studio}"
	}

	libdirs
	{
		"%{LibDir.fmod_core}",
		"%{LibDir.fmod_bank}",
		"%{LibDir.fmod_studio}"
	}

	links
	{
		"opengl32.lib",
		"glfw",
		"%{LibName.fmod_core}",
		"%{LibName.fmod_bank}",
		"%{LibName.fmod_studio}"
		
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
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/application/\""),
			("{COPY} $(SolutionDir)%{LibDir.fmod_core}/%{DllName.fmod_core} \"$(SolutionDir)bin/" .. outputdir .. "/application/\""),
			("{COPY} $(SolutionDir)%{LibDir.fmod_bank}/%{DllName.fmod_bank} \"$(SolutionDir)bin/" .. outputdir .. "/application/\""),
			("{COPY} $(SolutionDir)%{LibDir.fmod_bank}/%{DllName.fmod_bank2} \"$(SolutionDir)bin/" .. outputdir .. "/application/\""),
			("{COPY} $(SolutionDir)%{LibDir.fmod_studio}/%{DllName.fmod_studio} \"$(SolutionDir)bin/" .. outputdir .. "/application/\"") 
		}

	filter "configurations:Debug"
		buildoptions "/MTd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MT"
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
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stbi}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.jsoncpp}",
		"%{IncludeDir.fmod_core}",
		"%{IncludeDir.fmod_bank}",
		"%{IncludeDir.fmod_studio}"
	}

	links
	{
		"engine"
	}
	

	postbuildcommands
	{
		("{COPY} $(SolutionDir)application/asset \"$(SolutionDir)bin/" .. outputdir .. "/application/asset\"") 
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{

		}

	filter "configurations:Debug"
		buildoptions "/MTd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MT"
		optimize "On"
