/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: VertexArray.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "VertexArray.h"
#include "Renderer2D.h"
#include "engine/EngineCore.h"
#include "../platform/OpenGL/OpenGLVertexArray.h"


namespace gswy {

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer2D::GetAPI())
		{
			case RendererAPI::API::None: ASSERT(false, "RendererAPI::API::None is currently not supported!");
			case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLVertexArray>();
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}
}