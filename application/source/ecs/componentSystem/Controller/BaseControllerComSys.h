/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/24/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"
#include "level/GameLevelMapManager.h"
#include "ecs/components/HitPointCom.h"

namespace gswy
{
	class BaseControllerComSys : public BaseComponentSystem<GameObjectType> {
	public:
		BaseControllerComSys() {
		}

		virtual void Init() override 
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<BaseControllerComSys>(this, EventType::ATTACKBASE, &BaseControllerComSys::OnAttackBase);
		}

		virtual void Update(double dt) override
		{
			SyncRegisteredEntities();
			auto base = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
			auto baseHPCom = GetComponent<HitPointCom>(base);

			if (baseHPCom->IsDepleted())
			{
				if (!baseHPCom->IsDead())
				{
					// TODO: handle base death
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e1 = MemoryManager::Make_shared<DeathEvent>(base);
					queue->Publish(e1);
				}
			}
		}

		void OnAttackBase(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<AttackBaseEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

				auto base = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
				auto baseHPCom = GetComponent<HitPointCom>(base);
				baseHPCom->AddHitPoint(-event->m_damage);

				DEBUG_PRINT("Base HP: " + Str(baseHPCom->GetPercentageHP()*100) + "%");
			}
		}
	};
}