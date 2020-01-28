/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Engine.h
Purpose: Header file for the abstraction of the game loop.
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "engine/EngineCore.h"

namespace gswy {

	class Window;

	class ENGINE_API Engine {

	public:

		Engine();
		// Code review (Yohan 01/26/2019) : Make destructor virtual, so that derived class could call it upon destruction.
		virtual ~Engine();
		// Code review (Yohan 01/26/2019) : Make Run() virtual, so that derived class could override it.
		virtual void Run();

	// Code review (Yohan 01/26/2019) : Make private members to be protected instead, so that derived class could access them
	protected:
		Window* m_window;
		bool m_isRunning = true;
	};

	Engine* CreateEngineApplication();
}
