/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Buffer.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"

#include "Buffer.h"
#include "Renderer2D.h"
#include "engine/EngineCore.h"
#include "../platform/OpenGL/OpenGLBuffer.h"

namespace gswy {
	
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLIndexBuffer>(indices, size);
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer2D::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return MemoryManager::Make_shared<OpenGLFrameBuffer>(width, height);
		}

		ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}
}
