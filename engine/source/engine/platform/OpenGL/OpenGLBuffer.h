/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLBuffer.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "../../renderer/Buffer.h"

namespace gswy {
	
	class OpenGLVertexBuffer :public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UpdateBufferData(float* vertices, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }


	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
		std::vector<float> m_Vertices;
	};

	class OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLFrameBuffer :public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);

		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		inline uint32_t GetRendererID() const { return m_RendererID; }
		inline uint32_t GetRenderTargetID() const { return m_RenderTargetID; }

	private:
		uint32_t m_RendererID;
		uint32_t m_RenderTargetID;
		uint32_t m_DepthID;
	};
}