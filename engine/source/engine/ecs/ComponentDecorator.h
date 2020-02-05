#pragma once

#include "Entity.h"
#include "ComponentManager.h"

namespace gswy {

	template<typename ComponentType>
	class ComponentDecorator {

	public:
		ComponentDecorator(): m_owner(NULL), m_component(nullptr), m_manager(nullptr) {
		}

		ComponentDecorator(Entity owner, ComponentType* component, ComponentManager<ComponentType>* manager) {
			m_owner = owner;
			m_component = component;
			m_manager = manager;
		}

		void RemoveComponent() {
			m_manager->RemoveComponentFromEntity(m_owner);
		}

		ComponentType* operator->() const {
			return m_component;
		}

	private:
		Entity m_owner;
		ComponentType* m_component;
		ComponentManager<ComponentType>* m_manager;

	};


}