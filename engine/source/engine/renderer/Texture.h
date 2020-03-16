/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Texture.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "engine/EngineCore.h"

namespace gswy {
	
	class ENGINE_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void AttachToFrameBuffer() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual uint32_t GetRendererID() const = 0;
	};

	class ENGINE_API Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};
}