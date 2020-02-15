/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Engine.h
Purpose			: Header file for the abstraction of the game loop.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "engine/EngineCore.h"

namespace gswy {

	class Window;

	class ENGINE_API Engine {

	public:

		Engine();
		virtual ~Engine();
		virtual void Run();
		virtual void Update(double ts);

		static double TOTAL_TIME;

	protected:
		static Window* window;
		static bool isRunning;
	};

	Engine* CreateEngineApplication();
}
