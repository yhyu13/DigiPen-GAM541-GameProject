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
#include <glfw3.h>

#include <iostream>

namespace gswy {

	Engine::Engine(): m_isRunning(true) {
		m_window = Window::CreateWindow();
	}
	
	Engine::~Engine() {
		// Code review (Yohan 01/21/2019) : added deallocation of dynamically allocated member.
		delete m_window;
	}

	void Engine::Run() {
	
		double previousTime = (double)glfwGetTime();
		while (m_isRunning) {
			std::cout << "App is running..." << std::endl;

			//Calculate time
			double currentTime = (double)glfwGetTime();
			double elapsedTime = currentTime - previousTime;
			previousTime = currentTime;

			m_window->Update();
			Update(elapsedTime);
			m_isRunning = !m_window->ShouldExit();
		}
	
	}

	void Engine::Update(double ts) {

	}
}