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
		}

		~EntityManager() {
		}

		const Entity<EntityType> Create(EntityType type) {
			return Entity<EntityType>(m_entityCount++, type);
		}

		void Destroy(Entity<EntityType>& entity) {
		}

	protected:

	private:

		unsigned int m_entityCount;
	};
}
