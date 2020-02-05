#include <algorithm>

#include "BaseComponentSystem.h"

namespace gswy {

	BaseComponentSystem::BaseComponentSystem(): m_parentWorld(nullptr) {
	}

	void BaseComponentSystem::Init() {
	}

	void BaseComponentSystem::Update(double frameTime) {
	}

	void BaseComponentSystem::Render() {
	}

	void BaseComponentSystem::SetWorld(GameWorld* world) {
		m_parentWorld = world;
	}

	void BaseComponentSystem::AddEntity(const Entity& entity) {
		m_registeredEntities.push_back(entity);
	}

	void BaseComponentSystem::RemoveEntity(const Entity& entity) {
		m_registeredEntities.erase(
			std::remove(m_registeredEntities.begin(), m_registeredEntities.end(), entity)
		);
	}

	BitMaskSignature& BaseComponentSystem::GetSystemSignature() {
		return m_systemSignature;
	}

}