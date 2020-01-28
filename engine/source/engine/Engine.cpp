/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Engine.cpp
Purpose: Abstraction of the game loop.
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "Engine.h"
#include "engine/window/Window.h"

#include <iostream>

namespace gswy {

	Engine::Engine(): m_isRunning(true) {
		m_window = Window::CreateWindow();
	}
	
	Engine::~Engine() {
	}

	void Engine::Run() {
	
		while (m_isRunning) {
			std::cout << "App is running..." << std::endl;
			m_window->Update();
			m_isRunning = !m_window->ShouldExit();
		}
	
	}

}