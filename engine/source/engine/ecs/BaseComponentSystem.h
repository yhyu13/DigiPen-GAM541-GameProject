/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseComponentSystem.h
Purpose			: Contains declarations for component-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#pragma once
#include "engine/thread/Lock.h"
#include "Entity.h"
#include "BitMaskSignature.h"
#include "ComponentDecorator.h"

namespace gswy {

	template <typename EntityType>
	class GameWorld;

	/*
		Systems are the places where the actual game-code goes. The entities and
		components are simply data-storage units, systems actually work on that
		data. 

		Each system can specify which components it wishes to pay attention to.
		This information is stored in the form of a bit-mask signature. Entities
		which have the necessary components (entity's signatures matches the
		system's signature) will be registered with the system.

		Usage example:

		// declaring a component
		struct Position : gswy::BaseComponent<Position> {
			Position() = default;
			Position(float x) : x(x) {};
			float x;
		};

		// declaring a system
		class Wind : public gswy::BaseComponentSystem {
		public:
			Wind() {
				
				// wind system wants to pay attention to the Position component,
				// thus creating the signature that contains that information.
				m_systemSignature.AddComponent<Position>();
			}

			// actual game logic goes here
			virtual void Update(double frameTime) override {
			}
		};
	*/
	template <typename EntityType>
	class BaseComponentSystem {

	public:

		BaseComponentSystem::BaseComponentSystem() : m_parentWorld(nullptr) {
			m_flag.clear();
		}
		virtual ~BaseComponentSystem() = default;

		BaseComponentSystem(const BaseComponentSystem&) = delete;
		BaseComponentSystem& operator=(const BaseComponentSystem&) = delete;

		virtual void Init() {
		}

		virtual void Update(double frameTime) {
		}

		virtual void PreRenderUpdate(double frameTime) {
		}

		virtual void Render(double frameTime) {
		}

		virtual void Render2(double frameTime) {
		}

		virtual void PostRenderUpdate(double frameTime) {
		}

		virtual void RemoveAllEntities(double frameTime) {
		}

		template<class ComponentType>
		ComponentDecorator<ComponentType, EntityType> GetComponent(Entity<EntityType> entity)
		{
			ComponentDecorator<ComponentType, EntityType> com;
			m_parentWorld->Unpack(entity, com);
			return com;
		}
		
		void SetWorld(GameWorld<EntityType>* world) {
			m_parentWorld = world;
		}

		void SyncRegisteredEntities()
		{
			atomic_lock_guard lock(m_flag);
			{
				m_registeredEntities = m_bufferedRegisteredEntities;
			}
		}

		void AddEntity(const Entity<EntityType>& entity) {
			atomic_lock_guard lock(m_flag);
			{
				m_bufferedRegisteredEntities.push_back(entity);
			}
		}

		void RemoveEntity(const Entity<EntityType>& entity) {
			atomic_lock_guard lock(m_flag);
			{
				if (!m_bufferedRegisteredEntities.empty())
				{
					m_bufferedRegisteredEntities.erase(std::remove(m_bufferedRegisteredEntities.begin(), m_bufferedRegisteredEntities.end(), entity), m_bufferedRegisteredEntities.end());
				}
			}
		}

		BitMaskSignature& GetSystemSignature() {
			return m_systemSignature;
		}

	protected:
		std::vector<Entity<EntityType>> m_registeredEntities;
		std::vector<Entity<EntityType>> m_bufferedRegisteredEntities;
		BitMaskSignature m_systemSignature;
		GameWorld<EntityType>* m_parentWorld;
		std::atomic_flag m_flag;
	};
}
