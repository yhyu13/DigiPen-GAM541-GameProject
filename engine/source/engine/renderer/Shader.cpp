/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Shader.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Shader.h"
#include "Renderer2D.h"
#include "../platform/OpenGL/OpenGLShader.h"

namespace gswy {

	std::shared_ptr<Shader> Shader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		switch (Renderer2D::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLShader>(vertexShaderPath, fragmentShaderPath);
		}
		
		ASSERT(true, "Unknown RendererAPI");
		return nullptr;
	}

}
