/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Texture.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Texture.h"

#include "Renderer2D.h"
#include "../platform/OpenGL/OpenGLTexture.h"

namespace gswy {

	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None:   ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLTexture2D>(width, height);
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer2D::GetAPI())
		{
			case RendererAPI::API::None:   ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLTexture2D>(path);
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}
	
}