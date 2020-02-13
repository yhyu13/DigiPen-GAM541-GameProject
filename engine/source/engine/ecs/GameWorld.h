/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: GameWorld.h
Purpose			: Contains definitions for GameWorld functionality.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#pragma once

#include "ComponentManager.h"
#include "BaseComponentSystem.h"
#include "BitMaskSignature.h"
#include "EntityManager.h"
#include "ComponentDecorator.h"

namespace gswy {

	template <typename EntityType>
	class EntityDecorator;

	/*
		GameWorld is the abstraction for the internal workings of the ECS system.
		It provides communication among various parts of the ECS. In essence all the
		game world does is provide users simplified methods for interacting
		with the ECS system.
	*/
	template <typename EntityType>
	class GameWorld {

	public:

		explicit GameWorld(std::shared_ptr<EntityManager<EntityType>> manager) : m_entityManager(manager) {
		}

		~GameWorld() {
		}

		void Init() {
			for (auto&& system : m_systems) {
				system->Init();
			}
		}

		void Update(double frameTime) {
			for (auto&& system : m_systems) {
				system->Update(frameTime);
			}
		}

		void Render() {
			for (auto&& system : m_systems) {
				system->Render();
			}
		}

		EntityDecorator<EntityType> GenerateEntity(EntityType type) {
			return EntityDecorator(m_entityManager->Create(type), this);
		}

		void UpdateComponentSystems(const Entity<EntityType>& entity, const BitMaskSignature& oldMask) {
			BitMaskSignature updatedMask = m_entityMasks[entity];
			for (auto&& system : m_systems) {
				BitMaskSignature systemSignature = system->GetSystemSignature();
				if (updatedMask.IsNewMatch(oldMask, systemSignature)) {
					system->AddEntity(entity);
				}
				else if (updatedMask.IsNoLongerMatched(oldMask, systemSignature)) {
					system->RemoveEntity(entity);
				}
			}
		}

		void RegisterSystem(std::shared_ptr<BaseComponentSystem<EntityType>> system) {
			system->SetWorld(this);
			m_systems.push_back(system);
		}

		void RemoveEntity(Entity<EntityType> entity) {
			for (auto&& system : m_systems) {
				system->RemoveEntity(entity);
			}
			m_entityManager->Destroy(entity);
		}

		/*
			
		*/
		template<typename ComponentType>
		void AddComponent(const Entity<EntityType>& entity, ComponentType& component) {
			ComponentManager<ComponentType, EntityType>* manager = GetComponentManager<ComponentType>();
			manager->AddComponentToEntity(entity, component);
			BitMaskSignature currentMask = m_entityMasks[entity];
			m_entityMasks[entity].AddComponent<ComponentType>(); // update the component-mask for the entity once a new component has been added
			UpdateComponentSystems(entity, currentMask); // update the component-systems
		}

		template<typename ComponentType>
		void RemoveComponent(const Entity<EntityType>& entity) {
			ComponentManager<ComponentType, EntityType>* manager = GetComponentManager<ComponentType>();
			manager->RemoveComponentFromEntity(entity);
			BitMaskSignature oldMask = m_entityMasks[entity];
			m_entityMasks[entity].RemoveComponent<ComponentType>();
			UpdateComponentSystems(entity, oldMask);
		}

		/*
			Use this method to get desired component for an entity.
		*/
		template<typename ComponentType>
		void Unpack(Entity<EntityType> entity, ComponentDecorator<ComponentType, EntityType>& handle) {
			ComponentManager<ComponentType, EntityType>* manager = GetComponentManager<ComponentType>();
			handle = ComponentDecorator<ComponentType, EntityType>(entity, manager->GetComponentByEntity(entity), manager);
		}

	private:
		/*
			Get the component-manager for a given component-type.
			Example usage: GetComponentManager<ComponentType>();
		*/
		template<typename ComponentType>
		ComponentManager<ComponentType, EntityType>* GetComponentManager() {
			unsigned int family = GetComponentTypeIndex<ComponentType>();
			if (family >= m_componentManagers.size()) {
				m_componentManagers.resize(family + 1);
			}
			if (!m_componentManagers[family]) {
				m_componentManagers[family] = std::make_unique<ComponentManager<ComponentType, EntityType>>();
			}
			return static_cast<ComponentManager<ComponentType, EntityType>*> (m_componentManagers[family].get());
		}

	private:

		std::shared_ptr<EntityManager<EntityType>> m_entityManager;
		std::vector<std::shared_ptr<BaseComponentSystem<EntityType>>> m_systems;
		std::vector<std::shared_ptr<BaseComponentManager>> m_componentManagers;
		std::map<Entity<EntityType>, BitMaskSignature> m_entityMasks;

	};
}