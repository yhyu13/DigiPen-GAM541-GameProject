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
#include "engine/renderer/Animation.h"
#include "engine/exception/EngineException.h"
#include "engine/ecs/EntityDecorator.h"

namespace gswy
{
	/*
	Data class that stores references to owner entity type
	*/
	template<typename EntityType>
	struct OwnershiptCom : BaseComponent<OwnershiptCom<EntityType>> {
		
		OwnershiptCom()
		{
			m_entity = Entity<EntityType>(0, EntityType(0));
		}

		explicit OwnershiptCom(const Entity<EntityType>& entity)
		{
			m_entity = entity;
		}
		explicit OwnershiptCom(const EntityDecorator<EntityType>& entityDec)
		{
			m_entity = entityDec.GetEntity();
		}
		const Entity<EntityType>& GetEntity() {
			return m_entity;
		}
	private:
		Entity<EntityType> m_entity;
	};
}
