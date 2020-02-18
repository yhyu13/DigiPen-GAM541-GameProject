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

namespace gswy
{
	class WeaponComSys : public BaseComponentSystem<GameObjectType> {
	public:
		WeaponComSys() {
		}

		virtual void Init() override {
			m_spawnZOrder = 5000;
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<WeaponComSys>(this, EventType::FIREWEAPON, &WeaponComSys::OnFIREWEAPON);
		}

		virtual void Update(double dt) override
		{
			if (m_spawnZOrder > 9000) m_spawnZOrder = 5000;
		}

		void OnFIREWEAPON(Event<GameObjectType, EventType>* e) 
		{
			if (auto event = static_cast<FireWeaponEvent*>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				ComponentDecorator<TransformCom, GameObjectType> position;
				m_parentWorld->Unpack(event->m_entity, position);

				auto pos = position->GetPos();
				auto rot = position->GetRotation();

				PRINT(pos);
				PRINT(rot);

				switch (event->m_entity.m_type)
				{
				case GameObjectType::PLAYER:
				{
					static int num_spawn = 1;
					for (int i = 0; i < num_spawn; ++i)
					{
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FIREBALL);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
						auto weapon_rot = rot + RAND_F(0, 180) * DEG2RAD;
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder)), weapon_rot);
						transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
						weapon.AddComponent(transform);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25, 0.25));
						weapon.AddComponent(sprite);
						auto animCom = AnimationCom();
						animCom.Add("fireBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						animCom.GetCurrentAnimation()->SetAnimIdle(false);
						weapon.AddComponent(animCom);
						auto aabb = BodyCom();
						aabb.ChooseShape("AABB", 0.25, 0.25);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
					}
					++num_spawn;
				}
					break;
				default:
					break;
				}
			}
		}
	private:
		int m_spawnZOrder;
	};
}