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