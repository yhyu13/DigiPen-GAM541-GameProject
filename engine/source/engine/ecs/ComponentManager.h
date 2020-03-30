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
#include "Entity.h"
#include "engine/allocator/MemoryManager.h"

#define RESERVED_SIZE 2048

namespace gswy {

	template<typename EntityType>
	struct Entity;

	template<typename ComponentType>
	struct ComponentData {
		uint32_t m_size = 0;
		MyVector(ComponentType) m_data;

		ComponentData() {
			m_data.reserve(RESERVED_SIZE);
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
			m_entities.reserve(RESERVED_SIZE);
		}

		~ComponentManager() {
		}

		uint32_t AddComponentToEntity(Entity<EntityType> entity, ComponentType& component) {
			uint32_t index = m_components.m_size++;
			m_components.m_data.push_back(component);
			m_entitiesAndComponentIndexes[entity] = index;
			m_entities.push_back(entity);
			return index;
		}

		ComponentType* GetComponentByEntity(Entity<EntityType> entity) {
			if (m_entitiesAndComponentIndexes.find(entity) != m_entitiesAndComponentIndexes.end())
			{
				return &m_components.m_data.at(m_entitiesAndComponentIndexes[entity]);
			}
			else
			{
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Entity " + str2wstr(Str(entity)) + L" does not have " + str2wstr(Str(typeid(ComponentType).name())) + L" component!");
			}
		}

		/*
			Remove component from an entity.
			Every time we remove an entity, we update the component list by moving
			the last component-instance in the list to take the place of the removed
			component instance.
		*/
		void RemoveComponentFromEntity(Entity<EntityType> entity) {

			if (m_entitiesAndComponentIndexes.find(entity) == m_entitiesAndComponentIndexes.end())
				return;

			uint32_t index = m_entitiesAndComponentIndexes[entity];
			uint32_t lastIndex = --m_components.m_size;
			
			// move the component data from last index to the index of the component data just removed
			std::swap(m_components.m_data[index], m_components.m_data[lastIndex]);
			m_components.m_data.pop_back();
			
			// updating the mapping for the moved entity
			m_entitiesAndComponentIndexes[m_entities[lastIndex]] = index;
			std::swap(m_entities[index], m_entities[lastIndex]);
			m_entities.pop_back(); // reduce the m_entities as entities are being destroyed

			m_entitiesAndComponentIndexes.erase(entity);
		}

	private:

		/*
			Stores all the component instances in an array
		*/
		ComponentData<ComponentType> m_components;

		/*
			Maps the entity to the index of the component instance in the m_components
		*/
		std::map<Entity<EntityType>, uint32_t> m_entitiesAndComponentIndexes;

		/*
			Stores all entities indexed by the index of the component instance in m_components
		*/
		MyVector(Entity<EntityType>) m_entities;
	};

}