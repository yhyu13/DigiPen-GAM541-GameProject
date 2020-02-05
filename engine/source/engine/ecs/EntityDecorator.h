#pragma once

#include "Entity.h"
#include "ComponentDecorator.h"
#include "GameWorld.h"

namespace gswy {

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
