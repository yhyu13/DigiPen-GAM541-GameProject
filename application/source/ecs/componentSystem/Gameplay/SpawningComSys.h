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
#include "ecs/components/LifeTimeCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/components/TransformCom.h"

namespace gswy
{
	class SpawningComSys : public BaseComponentSystem<GameObjectType> {
	public:
		SpawningComSys() {
		}

		virtual void Init() override {
			m_spawnZOrder = 1000;
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<SpawningComSys>(this, EventType::SPAWN, &SpawningComSys::OnSPAWN);
		}

		virtual void Update(double dt) override
		{
			if (m_spawnZOrder > 5000) m_spawnZOrder = 1000;
		}

		void OnSPAWN(EventQueue<GameObjectType, EventType>::EventPtrType e)
		{
			if (auto event = static_pointer_cast<SpawnEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				switch (event->m_type)
				{
				case GameObjectType::ENEMY:
					SpawnEnemey();
					break;
				default:
					break;
				}
				
			}
		}

		/*
			Demo
		*/
		void SpawnEnemey()
		{
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY);
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			obj.AddComponent(TransformCom(RAND_F(-1, 1), RAND_F(-1, 1), Z_ORDER(m_spawnZOrder++)));
			auto animCom2 = AnimationCom();
			animCom2.Add("MobAnimation1", "Move");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(0.25, 0.25 / 70 * 50));
			obj.AddComponent(sprite);
			auto aabb1 = BodyCom();
			aabb1.ChooseShape("AABB", 0.25, 0.25 / 70 * 50);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom());
		}

	private:
		int m_spawnZOrder;
	};
}