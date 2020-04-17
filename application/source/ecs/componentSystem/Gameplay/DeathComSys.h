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
#include "level/GameLevelMapManager.h"

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

				switch (event->m_entity.m_type)
				{
				case GameObjectType::BASE:
				{
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e1 = MemoryManager::Make_shared<LoadLostEvent>();
					queue->Publish(e1);
				}
				break;
				case GameObjectType::PLAYER:
				{
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e1 = MemoryManager::Make_shared<LoadDiedEvent>();
					queue->Publish(e1);
					auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(false);
					queue->Publish(e2);
				}
					break;
				case GameObjectType::ENEMY_1: case GameObjectType::ENEMY_2: case GameObjectType::ENEMY_BOSS_1: case GameObjectType::ENEMY_BOSS_2:
				{
					auto body = GetComponent<BodyCom>(event->m_entity);
					glm::vec2 position = glm::vec2(body->GetPos().x, body->GetPos().y);
					auto addCoinEvent = MemoryManager::Make_shared<AddCoinEvent>(position);
					queue->Publish(addCoinEvent);

					// Self increment the level manager counter
					GameLevelMapManager::GetInstance()->m_Kill++;

					DEBUG_PRINT("ENEMY has died!");
					auto _e = MemoryManager::Make_shared<GCEvent>(event->m_entity);
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