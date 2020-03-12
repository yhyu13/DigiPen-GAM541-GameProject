/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/12/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/EntityDecorator.h"

namespace gswy
{
	/*
	Data class that stores references to owner entity type
	*/
	template<typename EntityType>
	struct ChildrenCom : BaseComponent<ChildrenCom<EntityType>> {

		ChildrenCom()
		{
			m_entity.clear();
		}
		explicit ChildrenCom(const Entity<EntityType>& entity)
		{
			m_entity.push_back(entity);
		}
		explicit ChildrenCom(EntityDecorator<EntityType>& entityDec)
		{
			m_entity.push_back(entityDec.GetEntity());
		}
		void AddEntity(EntityDecorator<EntityType>& entityDec)
		{
			m_entity.push_back(entityDec.GetEntity());
		}
		void AddEntity(const Entity<EntityType>& entity)
		{
			m_entity.push_back(entity);
		}
		const std::vector<Entity<EntityType>>& GetEntities() {
			return m_entity;
		}
		bool RemoveEntity(const Entity<EntityType>& entity)
		{
			if (auto it = std::find(m_entity.begin(), m_entity.end(), entity))
			{
				m_entity.erase(it);
				return true;
			}
			else
			{
				return false;
			}
		}
	private:
		std::vector<Entity<EntityType>> m_entity;
	};
}
