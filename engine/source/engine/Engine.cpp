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
#include "engine/input/Input.h"
#include "engine/audio/AudioManager.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <sstream>

namespace gswy {

	Engine::Engine(): m_isRunning(true) {
		m_window = Window::CreateWindow();
		AudioManager::GetInstance()->Init();
	}
	
	Engine::~Engine() {
		AudioManager::GetInstance()->Shutdown();
		delete m_window;
	}

	void Engine::Run() {
		FramerateController* rateController = FramerateController::GetInstance(60);
		Input* input = Input::GetInstance();
		while (m_isRunning) {
			rateController->FrameStart();

#ifdef _DEBUG
			std::stringstream stream;
			stream << "Frame Time: " << rateController->GetFrameTime() * 1000  << "ms";
			m_window->UpdateTitle(stream.str());
#endif
			m_window->Update(rateController->GetFrameTime());

			if (input->IsKeyPressed(GLFW_KEY_A)) {
				PRINT("KEY A PRESSED!");
			}

			if (input->IsKeyTriggered(GLFW_KEY_SPACE)) {
				PRINT("KEY SPACE TRIGGERED!");
			}

			if (input->IsKeyReleased(GLFW_KEY_A)) {
				PRINT("KEY A RELEASED!");
			}

			if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 PRESSED!");
			}

			if (input->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 RELEASED!");
			}

			std::stringstream stream1;
			stream1 << "cursor-x: " << input->GetMousePositionX() << "\t";
			stream1 << "cursor-y: " << input->GetMousePositionY();
			PRINT(stream1.str());

			Update(rateController->GetFrameTime());

			m_isRunning = !m_window->ShouldExit();
			rateController->FrameEnd();
		}
	}

	void Engine::Update(double ts)
	{

	}

}