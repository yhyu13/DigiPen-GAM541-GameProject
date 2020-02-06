/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EntityDecorator.cpp
Purpose			: Contains definitions for entity-decorator.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#include "EntityDecorator.h"
#include "Entity.h"
#include "GameWorld.h"

namespace gswy {

	EntityDecorator::EntityDecorator(Entity entity, GameWorld* world) : m_entity(entity), m_world(world) {
	}

	EntityDecorator::~EntityDecorator() {
	}


	void EntityDecorator::RemoveEntity() {
		m_world->RemoveEntity(m_entity);
	}

}