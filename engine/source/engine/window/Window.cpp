/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Window.cpp
Purpose			: Abstraction for window creation logic.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "engine/window/Window.h"
#include "engine/EngineCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gswy {


	int Window::width = 1280;
	int Window::height = 720;

	float GSWY_GetWindowWidth()
	{
		return Window::width;
	}

	float GSWY_GetWindowHeight()
	{
		return Window::height;
	}

	
	Window* Window::InitializeWindow(const WindowProperties& properties) {
		return new Window(properties);
	}

	void Window::Update(double dt) {
		glfwPollEvents();
	}

	void Window::Render()
	{
		glfwSwapBuffers(m_window);
	}

	void Window::UpdateTitle(std::string title) {
		glfwSetWindowTitle(m_window, title.c_str());
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_window);
	}

	bool Window::ShouldExit() {
		return glfwWindowShouldClose(m_window);
	}

	Window::Window(const WindowProperties& properties) {
		Init(properties);
	}

	Window::~Window() {
		Shutdown();
	}

	void Window::Init(const WindowProperties& properties) {

		width = properties.m_width;
		height = properties.m_height;
		m_windowProperties.m_width = properties.m_width;
		m_windowProperties.m_height = properties.m_height;
		m_windowProperties.m_title = properties.m_title;
		m_windowProperties.m_input = properties.m_input;

		int success = glfwInit();
		ASSERT(success < 0, "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if (properties.IsFullScreen)
		{
			//Full Screen Settings
			auto monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			// Update window width and height
			width = mode->width;
			height = mode->height;
			m_windowProperties.m_width = mode->width;
			m_windowProperties.m_height = mode->height;

			m_window = glfwCreateWindow(mode->width, mode->height, properties.m_title.c_str(), monitor, NULL);
			properties.m_input->SetMouseMaxPositions(mode->width, mode->height);
		}
		else
		{
			m_window = glfwCreateWindow(properties.m_width, properties.m_height, properties.m_title.c_str(), nullptr, nullptr);
			properties.m_input->SetMouseMaxPositions(properties.m_width, properties.m_height);
		}
		ASSERT(m_window == nullptr, "Failed to create window!");

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowProperties);

		success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success < 0, "Could not initialize Glad!");

		//Put info to Log
		ENGINE_INFO(" OpenGL Info:");
		ENGINE_INFO(" Vender: {0}", glGetString(GL_VENDOR));
		ENGINE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
		ENGINE_INFO(" Version: {0}", glGetString(GL_VERSION));

		// Key callback
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
			WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

			switch (action) {

			case GLFW_PRESS:
			{
				properties.m_input->UpdateKeyboardState(key, true, true);
				break;
			}

			case GLFW_RELEASE:
			{
				properties.m_input->UpdateKeyboardState(key, false, false);
				break;
			}

			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
			switch (action) {

			case GLFW_PRESS:
			{
				properties.m_input->UpdateMouseButtonState(button, true);
				break;
			}

			case GLFW_RELEASE:
			{
				properties.m_input->UpdateMouseButtonState(button, false);
				break;
			}
			default:
				break;
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double positionX, double positionY) {
			WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
			properties.m_input->UpdateCursorPosition(positionX, positionY);
		});
	}
}