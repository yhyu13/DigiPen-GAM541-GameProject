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

#include <iostream>


#ifdef ENGINE_BUILD_DLL
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif

#define ASSERT(x, ...) { if(!x) { std::cout << "Assertion failed: " << __VA_ARGS__; __debugbreak(); } }

#ifdef _DEBUG 
#define DEBUG_PRINT(x) std::cout << x << std::endl
#else
#define DEBUG_PRINT(x)
#endif // DEBUG

#define PRINT(x) std::cout << x << std::endl

