/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: ComponentDecorator.h
Purpose			: Contains declarations for component-decorator.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#pragma once

#include "Entity.h"
#include "ComponentManager.h"

namespace gswy {

	/*
		Component decorators are the wrapper constructs for components.
		These decorators help in creating useful abstractions for
		component specific operations.
	*/
	template<typename ComponentType, typename EntityType>
	class ComponentDecorator {

	public:
		ComponentDecorator()
			:
			m_component(nullptr),
			m_manager(nullptr)
		{
		}

		ComponentDecorator(Entity<EntityType> owner, ComponentType* component, ComponentManager<ComponentType, EntityType>* manager) {
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

		friend void swap(ComponentDecorator<ComponentType, EntityType>& x, ComponentDecorator<ComponentType, EntityType>& y)
		{
			std::swap(*x.m_component, *y.m_component)
		}

	private:
		Entity<EntityType> m_owner;
		ComponentType* m_component;
		ComponentManager<ComponentType, EntityType>* m_manager;

	};


}