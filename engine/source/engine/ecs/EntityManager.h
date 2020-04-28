/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EntityManager.h
Purpose			: Contains definitions for entity-manager.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#pragma once

#include "Entity.h"
#include "engine/exception/EngineException.h"
#include "engine/thread/Lock.h"

namespace gswy {

	/*
		Entity managers takes care of assigning entity-ids to entities
		during their creations. It makes sure that no two entities within
		the system share the same entity-id.
	*/
	template <typename EntityType>
	class EntityManager {

	public:

		EntityManager() : m_entityCount(0) {
			m_flag.clear();
		}

		~EntityManager() {
		}

		const Entity<EntityType> Create(EntityType type) {
			atomic_lock_guard lock(m_flag);
			m_typeToEntity[type].push_back(++m_entityCount);
			return Entity<EntityType>(m_entityCount, type);
		}

		void Destroy(Entity<EntityType>& entity) {
			atomic_lock_guard lock(m_flag);
			std::vector<unsigned int>& vec = m_typeToEntity[entity.m_type];
			vec.erase(std::remove(vec.begin(), vec.end(), entity.m_id), vec.end());
		}

		const std::vector<unsigned int> GetAllEntityIDWithType(EntityType type)
		{
			atomic_lock_guard lock(m_flag);
			if (m_typeToEntity.find(type) != m_typeToEntity.end())
			{
				return m_typeToEntity[type];
			}
			else
			{
				return std::vector<unsigned int>();
			}
		}

	private:
		std::atomic_flag m_flag;
		std::map<EntityType, std::vector<unsigned int>> m_typeToEntity;
		unsigned int m_entityCount;
	};
}
