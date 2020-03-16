/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLTexture.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once
#include "../../renderer/Texture.h"

namespace gswy {
	
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t  width, uint32_t  height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void AttachToFrameBuffer() const override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void SetData(void* data, uint32_t size) override;
		virtual uint32_t GetRendererID() const { return m_RendererID; }
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		uint32_t m_InternalFormat, m_DataFormat;
	};
}