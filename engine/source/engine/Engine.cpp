/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Engine.cpp
Purpose			: Abstraction of the game loop.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#include "Engine.h"
#include "engine/window/Window.h"
#include "engine/framerate-controller/FramerateController.h"

#include <iostream>

namespace gswy {

	Engine::Engine(): m_isRunning(true) {
		m_window = Window::CreateWindow();
	}
	
	Engine::~Engine() {
		delete m_window;
	}

	void Engine::Run() {
		FramerateController* rateController = FramerateController::GetInstance(60);
		while (m_isRunning) {
			rateController->FrameStart();
			// Code review (Yohan) : using debug print instead of plain std cout
			DEBUG_PRINT("App is running...");
			DEBUG_PRINT("Frame time: " << rateController->GetFrameTime());
			//std::cout << "App is running..." << std::endl;
			//std::cout << "Frame time: " << rateController->GetFrameTime();
			m_window->Update();
			m_isRunning = !m_window->ShouldExit();
			rateController->FrameEnd();
		}
		// Code review (Yohan) : FramerateController::GetInstance() retuns a static in data segment instead of a pointer on the heap,
		//						 we don't have to delete it here
		// delete rateController;
	}

}