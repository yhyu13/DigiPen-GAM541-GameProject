/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/17/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class DeathComSys : public BaseComponentSystem<GameObjectType> {
	public:
		DeathComSys() {
		}

		virtual void Init() override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<DeathComSys>(this, EventType::DEATH, &DeathComSys::OnDEATH);
		}

		void OnDEATH(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<DeathEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

				// Set entity to be inactive
				event->m_entity.m_active = false;

				switch (event->m_entity.m_type)
				{
				case GameObjectType::PLAYER:
					// TODO : Need proper handle of player death
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Player has died");
					break;
				case GameObjectType::ENEMY:
				{
					// TODO : Need proper handle of enemy death
					PRINT("ENEMY has died!");
					/*auto _e = MemoryManager::Make_shared<GCEvent>(event->m_entity);
					queue->Publish(_e);*/

					// Test code : Instead of calling GC on death, making enemies fade out in 1 sec
					auto _e = MemoryManager::Make_shared<FadeEvent>(event->m_entity, 1.f, 0.f, 1.f, EventType::GC);
					queue->Publish(_e);
				}
					break;
				default:
					break;
				}
			}
		}
	};
}