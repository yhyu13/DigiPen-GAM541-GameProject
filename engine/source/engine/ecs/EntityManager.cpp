#include "EntityManager.h"

namespace gswy {

	EntityManager::EntityManager(): m_entityId(0) {
	}

	EntityManager::~EntityManager() {
	}

	const Entity EntityManager::Create() {
		return Entity(++m_entityId);
	}

	void EntityManager::Destroy(Entity& entity) {
	}
}