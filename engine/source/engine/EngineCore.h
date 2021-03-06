/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EngineCore.h
Purpose			: Core utilities used throughout the engine.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once
#include <sstream>
#include "engine/logging/LoggingCore.h"

#ifdef ENGINE_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API 
#endif

#define NONCOPYABLE(Class) Class( const Class& ) = delete; Class( const Class&& ) = delete; \
							Class& operator=(const Class&) = delete; Class& operator=(const Class&&) = delete;

#define ASSERT(x, ...) { if(x == false) { ENGINE_CRITICAL("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }

#ifdef _DEBUG 
#define DEBUG_PRINT(x) ENGINE_DEBUG(Str(x))
#define PRINT(x) ENGINE_INFO(Str(x));
#else
#define DEBUG_PRINT(x)
#define PRINT(x) 
#endif // DEBUG

template <typename T>
std::string Str(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}