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

#include "engine/window/Window.h"
#include "engine/EngineCore.h"

#include <glad/glad.h>
#include <glfw3.h>

namespace gswy {
	
	Window* Window::CreateWindow(const WindowProperties& properties) {
		return new Window(properties);
	}

	void Window::Update() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
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

		m_windowProperties.m_width = properties.m_width;
		m_windowProperties.m_height = properties.m_height;
		m_windowProperties.m_title = properties.m_title;

		int success = glfwInit();
		ASSERT(success, "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(properties.m_width, properties.m_height, properties.m_title.c_str(), nullptr, nullptr);
		ASSERT(m_window, "Failed to create window!");

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowProperties);

		success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success, "Could not initialize Glad!");

		// TODO: registering callbacks for keyboard, mouse and other inputs in your application
	}

}