/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLVertexArray.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "OpenGLVertexArray.h"
#include "../../EngineCore.h"

#include <glad/glad.h>

namespace gswy {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case gswy::ShaderDataType::Float:    return GL_FLOAT;
		case gswy::ShaderDataType::Float2:   return GL_FLOAT;
		case gswy::ShaderDataType::Float3:   return GL_FLOAT;
		case gswy::ShaderDataType::Float4:   return GL_FLOAT;
		case gswy::ShaderDataType::Mat3:     return GL_FLOAT;
		case gswy::ShaderDataType::Mat4:     return GL_FLOAT;
		case gswy::ShaderDataType::Int:      return GL_INT;
		case gswy::ShaderDataType::Int2:     return GL_INT;
		case gswy::ShaderDataType::Int3:     return GL_INT;
		case gswy::ShaderDataType::Int4:     return GL_INT;
		case gswy::ShaderDataType::Bool:     return GL_BOOL;
		}

		ASSERT(true, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ASSERT(vertexBuffer->GetLayout().GetElements().size() == 0, "Vertex Buffer has no layout!")

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
