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

#include <map>
#include <array>

#include "Entity.h"

namespace gswy {

	template<typename ComponentType>
	struct ComponentData {
		unsigned int m_size = 0;
		std::array<ComponentType, 1024>* m_data;
	};

	class BaseComponentManager {
	public:
		BaseComponentManager() = default;
		virtual ~BaseComponentManager() = default;
		BaseComponentManager(const BaseComponentManager&) = default;
		BaseComponentManager& operator=(const BaseComponentManager&) = default;
	};

	template<typename ComponentType>
	class ComponentManager :public BaseComponentManager {

	public:

		ComponentManager() {
			m_components.m_data = static_cast<std::array<ComponentType, 1024>*>(malloc(sizeof(ComponentType) * 1024));
		}

		~ComponentManager() {
		}

		/*
			Add a component to an entity
		*/
		unsigned int AddComponentToEntity(Entity entity, ComponentType& component) {
			unsigned int index = m_components.m_size++;
			m_components.m_data->at(index) = component;
			m_entitiesAndComponentIndexes[entity] = index;
			m_entities[index] = entity;
			return index;
		}

		/*
			Get a component of an entity
		*/
		ComponentType* GetComponentByEntity(Entity entity) {
			unsigned int index = m_entitiesAndComponentIndexes[entity];
			return &m_components.m_data->at(index);
		}

		/*
			Remove component from an entity.
			Every time we remove an entity, we update the component data by moving
			the component-instance to take the place of the removed component instance.
		*/
		void RemoveComponentFromEntity(Entity entity) {
			unsigned int index = m_entitiesAndComponentIndexes[entity];
			unsigned int lastIndex = --m_components.m_size;

			// move the component data from last index to the index of the component data just removed
			m_components.m_data[index] = m_components.m_data[index];
			m_entitiesAndComponentIndexes.erase(entity);

			// updating the mapping for the moved entity
			Entity movedEntity = m_entities[lastIndex];
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
		std::map<Entity, unsigned int> m_entitiesAndComponentIndexes;

		/*
			Stores all entities indexed by the index of the component instance in m_components
		*/
		std::array<Entity, 1024> m_entities;
	};

}