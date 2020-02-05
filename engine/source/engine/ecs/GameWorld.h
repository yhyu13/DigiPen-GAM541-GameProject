#pragma once

#include <vector>
#include <map>

#include "ComponentManager.h"
#include "BaseComponentSystem.h"
#include "BitMaskSignature.h"
#include "EntityManager.h"
#include "ComponentDecorator.h"

namespace gswy {

	class EntityDecorator;

	class GameWorld {

	public:

		explicit GameWorld(std::unique_ptr<EntityManager> manager);
		~GameWorld();

		void Init();
		void Update(double frameTime);
		void Render();
		EntityDecorator GenerateEntity();
		void UpdateComponentSystems(const Entity& entity, const BitMaskSignature& oldMask);
		void RegisterSystem(std::unique_ptr<BaseComponentSystem> system);
		void RemoveEntity(Entity entityToBeRemoved);

		/*
			
		*/
		template<typename ComponentType>
		void AddComponent(const Entity& entity, ComponentType& component) {
			ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
			manager->AddComponentToEntity(entity, component);
			BitMaskSignature currentMask = m_entityMasks[entity];
			m_entityMasks[entity].AddComponent<ComponentType>(); // update the component-mask for the entity once a new component has been added
			UpdateComponentSystems(entity, currentMask); // update the component-systems
		}

		/*
			
		*/
		template<typename ComponentType>
		void RemoveComponent(const Entity& entity) {
			ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
			ComponentDecorator<ComponentType> component = manager->GetComponentByEntity(entity);
			component.RemoveComponent();
			BitMaskSignature oldMask = m_entityMasks[entity];
			m_entityMasks[entity].RemoveComponent<ComponentType>();
			UpdateComponentSystems(entity, oldMask);
		}

		/*
			
		*/
		template<typename ComponentType, typename... Args>
		void Unpack(Entity entity, ComponentDecorator<ComponentType>& handle, ComponentDecorator<Args> &... args) {
			typedef ComponentManager<ComponentType> ComponentManagerType;
			ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
			handle = ComponentDecorator<ComponentType>(entity, manager->GetComponentByEntity(entity), manager);
			Unpack<Args ...>(entity, args...);
		}

		/*
			
		*/
		template<typename ComponentType>
		void Unpack(Entity entity, ComponentDecorator<ComponentType>& handle) {
			ComponentManager<ComponentType>* manager = GetComponentManager<ComponentType>();
			handle = ComponentDecorator<ComponentType>(entity, manager->GetComponentByEntity(entity), manager);
		}

	private:

		std::unique_ptr<EntityManager> m_entityManager;
		std::vector<std::unique_ptr<BaseComponentSystem>> m_systems;
		std::vector<std::unique_ptr<BaseComponentManager>> m_componentManagers;
		std::map<Entity, BitMaskSignature> m_entityMasks;

		/*
			
		*/
		template<typename ComponentType>
		ComponentManager<ComponentType>* GetComponentManager() {
			unsigned int family = GetComponentTypeIndex<ComponentType>();
			if (family >= m_componentManagers.size()) {
				m_componentManagers.resize(family + 1);
			}
			if (!m_componentManagers[family]) {
				m_componentManagers[family] = std::make_unique<ComponentManager<ComponentType>>();
			}
			return static_cast<ComponentManager<ComponentType>*> (m_componentManagers[family].get());
		}
	};
}