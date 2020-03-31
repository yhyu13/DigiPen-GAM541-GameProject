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
IncludeDir["spdlog"] 		= "engine/vendors/spdlog/include"
IncludeDir["ImGui"]      =   "engine/vendors/imgui"
IncludeDir["tileson"]      ="engine/vendors/tileson/include"

LibDir = {}
LibDir["glfw"] = "engine/vendors/glfw"
LibDir["fmod_core"] = "engine/vendors/fmod/api/core/lib/x64"
LibDir["fmod_bank"] = "engine/vendors/fmod/api/fsbank/lib/x64"
LibDir["fmod_studio"] = "engine/vendors/fmod/api/studio/lib/x64"
LibDir["tileson"]      ="engine/vendors/tileson/lib/x64"

LibName = {}
LibName["fmod_core"] = "fmod_vc.lib"
LibName["fmod_bank"] = "fsbank_vc.lib"
LibName["fmod_studio"] = "fmodstudio_vc.lib"
LibName["tileson"] = "tileson.lib"
DllName = {}
DllName["fmod_core"] = "fmod.dll"
DllName["fmod_bank"] = "fsbank.dll"
DllName["fmod_bank2"] = "libfsbvorbis64.dll"
DllName["fmod_studio"] = "fmodstudio.dll"

include "engine/vendors/glfw"
include "engine/vendors/imgui"

project "engine"
	location "engine"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")
	
	pchheader "engine-precompiled-header.h"
	pchsource "engine/source/engine-precompiled-header.cpp"

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
		"%{prj.name}/vendors/jsoncpp/src/lib_json/**.inl",
		"%{prj.name}/vendors/tileson/include/**.h",
		"%{prj.name}/vendors/tileson/include/**.hpp",
		"%{prj.name}/vendors/tileson/include/**.cpp",
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
		"%{IncludeDir.fmod_studio}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.tileson}"
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
		"ImGui",
		"%{LibName.fmod_core}",
		"%{LibName.fmod_bank}",
		"%{LibName.fmod_studio}"
	}
	
	filter { "files:**.c" }
		compileas "C++"

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
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MD"
		optimize "On"

project "application"
	location "application"
	kind "WindowedApp"
	language "C++"
	icon "example.ico"
	targetname ("Light of Empyrion")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"application/source",
		"engine/source",
		"engine/vendors",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stbi}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.jsoncpp}",
		"%{IncludeDir.fmod_core}",
		"%{IncludeDir.fmod_bank}",
		"%{IncludeDir.fmod_studio}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.tileson}"
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
		files { 'resources.rc', '**.ico' }
		vpaths { ['/*'] = { '*.rc', '**.ico' } }
		defines
		{

		}

	filter "configurations:Debug"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		buildoptions "/MD"
		optimize "On"
