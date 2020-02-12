/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: ComponentManager.h
Purpose			: Contains declarations for component-manager.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#pragma once

#include "Entity.h"

namespace gswy {

	template<typename ComponentType>
	struct ComponentData {
		unsigned int m_size = 0;
		std::vector<ComponentType> m_data;

		ComponentData() {
			m_data.reserve(1024);
		}
	};

	class BaseComponentManager {
	public:
		BaseComponentManager() = default;
		virtual ~BaseComponentManager() = default;
		BaseComponentManager(const BaseComponentManager&) = default;
		BaseComponentManager& operator=(const BaseComponentManager&) = default;
	};


	/*
		Component manager brings the entities and their corresponding components together.
		Each component-type gets a component-manager.

		Instead of making entities own their components, it is much more cache efficient
		to store all components of a type at a single location. This ensures that when
		systems run their update methods, they access all required components of a type
		from a contiguous memory.
	*/
	template<typename ComponentType, typename EntityType>
	class ComponentManager :public BaseComponentManager {

	public:

		ComponentManager() {
			m_entities.reserve(1024);
		}

		~ComponentManager() {
		}

		unsigned int AddComponentToEntity(Entity<EntityType> entity, ComponentType& component) {
			unsigned int index = m_components.m_size++;
			m_components.m_data.push_back(component);
			m_entitiesAndComponentIndexes[entity] = index;
			m_entities.push_back(entity);
			return index;
		}

		ComponentType* GetComponentByEntity(Entity<EntityType> entity) {
			unsigned int index = m_entitiesAndComponentIndexes[entity];
			return &m_components.m_data.at(index);
		}

		/*
			Remove component from an entity.
			Every time we remove an entity, we update the component list by moving
			the last component-instance in the list to take the place of the removed
			component instance.
		*/
		void RemoveComponentFromEntity(Entity<EntityType> entity) {
			unsigned int index = m_entitiesAndComponentIndexes[entity];
			unsigned int lastIndex = --m_components.m_size;

			// move the component data from last index to the index of the component data just removed
			m_components.m_data[index] = m_components.m_data[index];
			m_entitiesAndComponentIndexes.erase(entity);

			// updating the mapping for the moved entity
			Entity<EntityType> movedEntity = m_entities[lastIndex];
			m_entitiesAndComponentIndexes[movedEntity] = index;
			m_entities[index] = movedEntity;
		}

	private:

		/*
			Stores all the component instances in an array
		*/
		ComponentData<ComponentType> m_components;

		/*
			Maps the entity to the index of the component instance in the m_components
		*/
		std::map<Entity<EntityType>, unsigned int> m_entitiesAndComponentIndexes;

		/*
			Stores all entities indexed by the index of the component instance in m_components
		*/
		std::vector<Entity<EntityType>> m_entities;
	};

}