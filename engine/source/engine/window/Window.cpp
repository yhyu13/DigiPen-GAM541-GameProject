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
#include "engine/renderer/Renderer2D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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

	
	Window* Window::InitializeWindow(const Json::Value& windowConfiguration) {
		return new Window(windowConfiguration);
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

	void Window::ToggleFullScreen(bool on)
	{
		if (on)
		{
			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
			glfwWindowHint(GLFW_DECORATED, false);
			glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY, GLFW_DONT_CARE);
			
			int w, h;
			glfwGetWindowSize(m_window, &w, &h);
			m_windowProperties.m_width = w;
			m_windowProperties.m_height = h;

			printf("Full Screen : width = %d, height = %d\n", w, h);

			int l, r, t, b;
			glfwGetWindowFrameSize(m_window, &l, &t, &r, &b);
			printf("Frame Size : left = %d, top = %d right = %d, bottom = %d\n", l, t, r, b);
		}
		else
		{
			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
			glfwWindowHint(GLFW_DECORATED, true);
			int x = 0, y = 0, w = 1, h = 1;
			glfwSetWindowMonitor(m_window, nullptr, x, y, w, h, GLFW_DONT_CARE);

			int l, r, t, b;
			glfwGetWindowFrameSize(m_window, &l, &t, &r, &b);
			printf("Frame Size : left = %d, top = %d right = %d, bottom = %d\n", l, t, r, b);

			//int x, y, w, h;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
			glfwSetWindowMonitor(m_window, nullptr, x, y, m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY, GLFW_DONT_CARE);

			m_windowProperties.m_width = m_windowProperties.m_resolutionX;
			m_windowProperties.m_height = m_windowProperties.m_resolutionY;
			printf("No Full Screen : posX = %d, posY = %d width = %d, height = %d\n", x, y, w, h);

		}
		m_windowProperties.IsFullScreen = on;
		width = m_windowProperties.m_width;
		height = m_windowProperties.m_height;
		printf("Window Size : %d x %d\n", width, height);

		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		printf("Framebuffer Size : %d x %d\n", w, h);

		m_windowProperties.m_input->SetMouseMaxPositions(m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY);
	}

	void Window::SetResolution(int n)
	{
		switch (n)
		{
			case 0:
			{
				m_windowProperties.m_resolutionX = m_windowProperties.m_Res1.first;
				m_windowProperties.m_resolutionY = m_windowProperties.m_Res1.second;
			}
				break;

			case 1:
			{
				m_windowProperties.m_resolutionX = m_windowProperties.m_Res2.first;
				m_windowProperties.m_resolutionY = m_windowProperties.m_Res2.second;
			}
				break;

			case 2:
			{
				m_windowProperties.m_resolutionX = m_windowProperties.m_Res3.first;
				m_windowProperties.m_resolutionY = m_windowProperties.m_Res3.second;
			}
				break;

			case 3:
			{
				m_windowProperties.m_resolutionX = m_windowProperties.m_Res4.first;
				m_windowProperties.m_resolutionY = m_windowProperties.m_Res4.second;
			}
				break;

			default:
			{
				m_windowProperties.m_resolutionX = m_windowProperties.m_width;
				m_windowProperties.m_resolutionY = m_windowProperties.m_height;
			}
				break;
		}

		if (m_windowProperties.IsFullScreen)
		{
			glfwWindowHint(GLFW_DECORATED, false);
			glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY, GLFW_DONT_CARE);
		}
		else
		{
			int x, y;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, nullptr, nullptr);
			glfwSetWindowMonitor(m_window, nullptr, x, 6, m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY, GLFW_DONT_CARE);
		}
		m_windowProperties.m_input->SetMouseMaxPositions(m_windowProperties.m_resolutionX, m_windowProperties.m_resolutionY);
		width = m_windowProperties.m_resolutionX;
		height = m_windowProperties.m_resolutionY;
	}

	void Window::SetVSync(bool on)
	{
		if (on)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_window);
	}

	bool Window::ShouldExit() {
		return glfwWindowShouldClose(m_window);
	}

	Window::Window(const Json::Value& windowConfiguration) {
		Init(windowConfiguration);
	}

	Window::~Window() {
		Shutdown();
	}

	void Window::Init(const Json::Value& windowConfiguration) {

		width = windowConfiguration["width"].asInt();
		height = windowConfiguration["height"].asInt();
		m_windowProperties.m_width = windowConfiguration["width"].asInt();
		m_windowProperties.m_height = windowConfiguration["height"].asInt();
		m_windowProperties.m_title = windowConfiguration["title"].asString();
		m_windowProperties.IsFullScreen = windowConfiguration["full-screen"].asBool();

		m_windowProperties.m_input = InputManager::GetInstance();
		m_windowProperties.m_input->SetMouseMaxPositions(width, height);

		m_windowProperties.m_Res1 = { windowConfiguration["Resolution1_X"].asInt(), windowConfiguration["Resolution1_Y"].asInt() };
		m_windowProperties.m_Res2 = { windowConfiguration["Resolution2_X"].asInt(), windowConfiguration["Resolution2_Y"].asInt() };
		m_windowProperties.m_Res3 = { windowConfiguration["Resolution3_X"].asInt(), windowConfiguration["Resolution3_Y"].asInt() };
		m_windowProperties.m_Res4 = { windowConfiguration["Resolution4_X"].asInt(), windowConfiguration["Resolution4_Y"].asInt() };

		int success = glfwInit();
		ASSERT(success >= 0, "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		m_windowProperties.m_monitorWidth = mode->width;
		m_windowProperties.m_monitorHeight = mode->height;

		m_windowProperties.m_resolutionX = m_windowProperties.m_width;
		m_windowProperties.m_resolutionY = m_windowProperties.m_height;

		if (m_windowProperties.IsFullScreen)
		{
			//Full Screen Settings
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			// Update window width and height
			width = mode->width;
			height = mode->height;
			//m_windowProperties.m_width = mode->width;
			//m_windowProperties.m_height = mode->height;

			m_window = glfwCreateWindow(mode->width, mode->height, m_windowProperties.m_title.c_str(), monitor, nullptr);
			m_windowProperties.m_input->SetMouseMaxPositions(mode->width, mode->height);
		}
		else
		{
			m_window = glfwCreateWindow(m_windowProperties.m_width, m_windowProperties.m_height, m_windowProperties.m_title.c_str(), nullptr, nullptr);
			m_windowProperties.m_input->SetMouseMaxPositions(m_windowProperties.m_width, m_windowProperties.m_height);
		}
		ASSERT(m_window != nullptr, "Failed to create window!");

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowProperties);

		success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(success >= 0, "Could not initialize Glad!");

		//Put info to Log
		ENGINE_INFO(" OpenGL Info:");
		ENGINE_INFO(" Vendor: {0}", glGetString(GL_VENDOR));
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

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
			properties.m_width = width;
			properties.m_height = height;
			properties.m_input->SetMouseMaxPositions(width, height);
			Renderer2D::OnWindowResize(width, height);
		});

		glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focussed)
		{
			WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
			properties.m_interruptHandler(focussed);
		});
	}
}