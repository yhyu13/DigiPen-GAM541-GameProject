/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Window.h
Purpose			: Header file for the abstraction for window creation logic.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "engine/EngineCore.h"
#include "engine/input/InputManager.h"

struct GLFWwindow;

namespace gswy {

	struct ENGINE_API WindowProperties {
		unsigned int m_width;
		unsigned int m_height;
		std::string m_title;
		InputManager* m_input = nullptr;
		bool IsFullScreen = false;

		WindowProperties(unsigned int width = 1280, unsigned int height = 720, std::string title = "ENGINE GSWY") : m_width(width), m_height(height), m_title(title) {
			m_input = InputManager::GetInstance();
			m_input->SetMouseMaxPositions(width, height);
		}
	};

	float GSWY_GetWindowWidth();
	float GSWY_GetWindowHeight();

	class ENGINE_API Window {

	public:

		~Window();

		static Window* InitializeWindow(const WindowProperties& properties = WindowProperties());

		void Update(double dt);
		void Render();
		void Shutdown();
		bool ShouldExit();
		void UpdateTitle(std::string title);

		GLFWwindow* GetNativeWindow() const { return m_window; }
		inline unsigned int GetWidth() const { return m_windowProperties.m_width; }
		inline unsigned int GetHeight() const { return m_windowProperties.m_height; }

	protected:

	private:

		Window(const WindowProperties& properties);

		void Init(const WindowProperties& properties);

	public:
		static int width;
		static int height;
	protected:


	private:

		WindowProperties m_windowProperties;
		GLFWwindow* m_window;
	};
}