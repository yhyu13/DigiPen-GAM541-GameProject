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

		void OnDEATH(Event<GameObjectType, EventType>* e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_cast<DeathEvent*>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
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
					GCEvent _e(event->m_entity);
					queue->Publish(&_e);
				}
					break;
				default:
					break;
				}
			}
		}
	};
}