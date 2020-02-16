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

#include "engine-precompiled-header.h"
#include "Engine.h"
#include "engine/window/Window.h"
#include "engine/framerate-controller/FramerateController.h"
#include "engine/input/InputManager.h"
#include "engine/audio/AudioManager.h"
#include "engine/allocator/MemoryManager.h"

#include <GLFW/glfw3.h>

namespace gswy {

	double Engine::TOTAL_TIME = 0.0;
	bool Engine::isRunning = true;
	Engine* Engine::s_instance = nullptr;

	Engine::Engine() 
	{
		Logger::Init();
		ENGINE_INFO("Initialized Engine Log!");
		APP_INFO("Initialized Application Log!");

		s_instance = this;
		window = std::unique_ptr<Window>(Window::InitializeWindow());
		MemoryManager::GetInstance()->Init();
		AudioManager::GetInstance()->Init();

		// initializing imgui layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	
	Engine::~Engine() {
		AudioManager::GetInstance()->Shutdown();
		MemoryManager::GetInstance()->Shutdown();
	}

	void Engine::Run() {
		FramerateController* rateController = FramerateController::GetInstance(60);
		while (isRunning) {
			rateController->FrameStart();

#ifdef _DEBUG
			std::stringstream stream;
			stream << "Frame Time: " << rateController->GetFrameTime() * 1000  << "ms";
			window->UpdateTitle(stream.str());
#endif
			Update(rateController->GetFrameTime());

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(rateController->GetFrameTime());

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			isRunning = !window->ShouldExit();
			rateController->FrameEnd();
		}
	}

	void Engine::Update(double ts)
	{
		TOTAL_TIME += ts;
		// window update (need to be called in at the beginning of each frame)
		window->Update(ts);
		MemoryManager::GetInstance()->Update(ts);
		AudioManager::GetInstance()->Update(ts);
	}

	void Engine::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Engine::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

}