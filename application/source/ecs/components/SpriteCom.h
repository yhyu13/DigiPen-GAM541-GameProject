/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Sprite.h"

namespace gswy
{
	/* Data class of sprite */
	struct SpriteCom : BaseComponent<SpriteCom> {

		SpriteCom()
		{
			m_sprite = MemoryManager::Make_shared<Sprite>();
		}

		void SetTexture(const std::string& name)
		{
			m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get(name));
			m_name = name;
		}

		const std::string& GetTextureName()
		{
			return m_name;
		}

		void SetScale(const glm::vec2& t)
		{
			m_sprite->SetSpriteScale(t);
		}

		void SetAlpha(float a)
		{
			m_sprite->SetSpriteAlpha(a);
		}

		std::shared_ptr<Sprite> Get()
		{
			return m_sprite;
		}
	private:
		std::shared_ptr<Sprite> m_sprite;
		std::string m_name;
	};
}
