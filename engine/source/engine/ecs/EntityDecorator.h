/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EntityDecorator.h
Purpose			: Contains declarations for entity-decorator.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#pragma once

#include "Entity.h"
#include "ComponentDecorator.h"
#include "GameWorld.h"

namespace gswy {

	/*
		Entity decorators are the wrapper constructs for entities.
		These decorators help in creating useful abstractions for
		entity specific operations.
	*/
	class EntityDecorator {

	public:

		EntityDecorator(Entity entity, GameWorld* world);
		~EntityDecorator();

		void RemoveEntity();

		template<typename ComponentType>
		void AddComponent(ComponentType& component) {
			m_world->AddComponent<ComponentType>(m_entity, component);
		}

		template<typename ComponentType>
		void RemoveComponent() {
			m_world->RemoveComponent<ComponentType>(m_entity);
		}

		template<typename ComponentType>
		ComponentDecorator<ComponentType> GetComponent() {
			ComponentDecorator<ComponentType> handle;
			m_world.Unpack(entity, handle);
			return handle;
		}

	protected:

	private:

		Entity m_entity;
		GameWorld* m_world;
	};
}