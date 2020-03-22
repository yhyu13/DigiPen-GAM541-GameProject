/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLContext.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "OpenGLContext.h"
#include "engine/EngineCore.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace gswy {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ASSERT(windowHandle == nullptr, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Failed to initialize Glad");
		
		//Put info to Log
		ENGINE_INFO(" OpenGL Info:");
		ENGINE_INFO(" Vender: {0}", glGetString(GL_VENDOR));
		ENGINE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
		ENGINE_INFO(" Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}

