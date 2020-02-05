/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseComponentSystem.h
Purpose			: Contains definitions for component-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

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