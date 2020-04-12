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
#include "layer/LayerStack.h"
#include "ui/ImGuiLayer.h"

namespace gswy {

	class Window;

	class ENGINE_API Engine {

	public:

		Engine();
		virtual ~Engine();
		virtual void Run();
		virtual void Update(double ts);
		virtual void LateUpdate(double ts);

		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *window; }
		inline static Engine& Get() { return *s_instance; }
		static double TOTAL_TIME;
		static void SetQuit(bool b) { isRunning = !b; }
	public:
		//static Window* window;
		static bool isRunning;
		static bool isPaused;

	private:
		std::unique_ptr<Window> window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		double m_StartTime;
		unsigned int m_MaxFrameRate;

		static Engine* s_instance;

	};

	Engine* CreateEngineApplication();
}
