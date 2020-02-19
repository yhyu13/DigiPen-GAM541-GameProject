/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/18/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/HitPointCom.h"
#include "ecs/components/HitPreventionCom.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class HitPointComSys : public BaseComponentSystem<GameObjectType> {
	public:
		HitPointComSys() {
			m_systemSignature.AddComponent<HitPointCom>();
		}

		virtual void Init() override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<HitPointComSys>(this, EventType::COLLISION, &HitPointComSys::OnCOLLISION);
		}

		virtual void Update(double dt) override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				m_parentWorld->Unpack(entity, HitPoint);
				if (HitPoint->IsDepleted())
				{
					DeathEvent e(entity);
					queue->Publish(&e);
				}
			}
		}

		void OnCOLLISION(Event<GameObjectType, EventType>* e)
		{
			if (auto event = static_cast<CollisionEvent*>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				bool isSwasp = false;
				auto entityA = event->m_entityA;
				auto entityB = event->m_entityB;
				BEGIN:
				switch (entityA.m_type)
				{
				case GameObjectType::PLAYER:
					PLAYER(entityA, entityB);
					return;
				case GameObjectType::ENEMY:
					ENEMY(entityA, entityB);
					return;
				default:
					break;
				}
				if (!isSwasp)
				{
					// Sawp A,B
					std::swap(entityA, entityB);
					isSwasp = true;
					goto BEGIN;
				}
				return;
			}
		}

		void PLAYER(const Entity<GameObjectType>& entityA, const Entity<GameObjectType>& entityB)
		{
			switch (entityB.m_type)
			{
			case GameObjectType::ENEMY:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				m_parentWorld->Unpack(entityA, HitPoint);
				//HitPoint->AddHitPoint(-1);
			}	
				break;
			default:
				break;
			}
		}

		void ENEMY(const Entity<GameObjectType>& entityA, const Entity<GameObjectType>& entityB)
		{
			switch (entityB.m_type)
			{
			case GameObjectType::FIREBALL:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityA, HitPrevention);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-10);
				}
			}
				break;
			default:
				break;
			}
		}
	};
}