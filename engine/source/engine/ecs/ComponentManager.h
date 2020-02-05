#pragma once

#include <map>
#include <array>

#include "engine/EngineCore.h"
#include "Entity.h"

namespace gswy {

	/*struct ComponentIndex {
		unsigned int m_index;

		ComponentIndex(int index) {
			m_index = index;
		}
	};*/

	template<typename ComponentType>
	struct ENGINE_API ComponentData {
		unsigned int m_size = 0;
		std::array<ComponentType, 1024>* m_data;
	};

	class ENGINE_API BaseComponentManager {
	public:
		BaseComponentManager() = default;
		virtual ~BaseComponentManager() = default;
		BaseComponentManager(const BaseComponentManager&) = default;
		BaseComponentManager& operator=(const BaseComponentManager&) = default;
	};

	template<typename ComponentType>
	class ENGINE_API ComponentManager :public BaseComponentManager {

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