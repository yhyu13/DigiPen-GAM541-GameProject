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

#include <json/json.h>

struct GLFWwindow;

namespace gswy {

	typedef std::function<void(const int& isFocussed)> InterruptHandler;

	struct ENGINE_API WindowProperties {
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_resolutionX;
		unsigned int m_resolutionY;
		std::string m_title;
		InputManager* m_input = nullptr;
		bool IsFullScreen = false;
		InterruptHandler m_interruptHandler;
		bool IsVSync = false;

		std::pair<unsigned int, unsigned int> m_Res1;
		std::pair<unsigned int, unsigned int> m_Res2;
		std::pair<unsigned int, unsigned int> m_Res3;
		std::pair<unsigned int, unsigned int> m_Res4;

		unsigned int m_monitorWidth;
		unsigned int m_monitorHeight;
		//Pre-Calculate number for some reasons
		//int m_windowFrameSize_top = 38;
		//int m_windowFrameSize_bottom = 9;

		WindowProperties(unsigned int width = 1280, unsigned int height = 720, std::string title = "ENGINE GSWY") : m_width(width), m_height(height), m_title(title), m_resolutionX(width), m_resolutionY(height){
			m_input = InputManager::GetInstance();
			m_input->SetMouseMaxPositions(width, height);
		}
	};

	float GSWY_GetWindowWidth();
	float GSWY_GetWindowHeight();

	class ENGINE_API Window {

	public:

		~Window();

		static Window* InitializeWindow(const Json::Value& engineConfiguration);

		void Update(double dt);
		void Render();
		void Shutdown();
		bool ShouldExit();
		void UpdateTitle(std::string title);
		void ToggleFullScreen(bool on);
		void SetResolution(int n);
		void SetVSync(bool on);

		GLFWwindow* GetNativeWindow() const { return m_window; }
		WindowProperties GetWindowProperties() const { return m_windowProperties; }
		inline unsigned int GetWidth() const { return m_windowProperties.m_width; }
		inline unsigned int GetHeight() const { return m_windowProperties.m_height; }


		inline void SetInterruptHandler(const InterruptHandler& handler) { m_windowProperties.m_interruptHandler = handler; };

	protected:

	private:

		Window(const Json::Value& engineConfiguration);

		void Init(const Json::Value& windowConfiguration);

	public:
		static int width;
		static int height;
	protected:


	private:

		WindowProperties m_windowProperties;
		GLFWwindow* m_window;
	};
}