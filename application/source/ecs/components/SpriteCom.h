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
	struct SpriteCom : BaseComponent<SpriteCom> {

		explicit SpriteCom(int id)
		{
			m_sprite = ResourceAllocator<Sprite>::GetInstance()->Get(id);
		}
		explicit SpriteCom(std::string name)
		{
			m_sprite = ResourceAllocator<Sprite>::GetInstance()->Get(name);
		}
		Sprite* Get()
		{
			return m_sprite;
		}
	private:
		Sprite* m_sprite;
	};
}
