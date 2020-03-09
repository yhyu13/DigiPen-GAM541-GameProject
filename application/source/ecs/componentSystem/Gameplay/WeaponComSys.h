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
#include "ecs/components/AttachedMovementCom.h"
#include "ecs/components/ParticleCom.h"
#include "ecs/components/LifeTimeCom.h"
#include "ecs/components/HitPointCom.h"
#include "ecs/components/HitPreventionCom.h"
#include "ecs/CustomEvents.h"

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

		void OnFIREWEAPON(EventQueue<GameObjectType, EventType>::EventPtrType e)
		{
			if (auto event = static_pointer_cast<FireWeaponEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(event->m_entity, transform);

				auto pos = transform->GetPos();
				auto rot = transform->GetRotation();

				switch (event->m_entity.m_type)
				{
				case GameObjectType::PLAYER:
				{
					static int num_spawn = 1;
					for (int i = 0; i < num_spawn; ++i)
					{
						{
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FIREBALL);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
							auto weapon_rot = rot + RAND_F(-90, 90) * DEG2RAD;
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
							weapon.AddComponent(transform);
							auto particle = ParticleCom();
							particle.Init<ExplosionParticle>();
							weapon.AddComponent(particle);
							auto animCom = AnimationCom();
							animCom.Add("fireBallAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							animCom.GetCurrentAnimation()->SetAnimIdle(false);
							weapon.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 0.25));
							weapon.AddComponent(sprite);
							auto aabb = BodyCom();
							aabb.ChooseShape("Circle", 0.1);
							weapon.AddComponent(aabb);
							weapon.AddComponent(LifeTimeCom(2.0));
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
													
						}
						{
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::ICEBALL);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
							auto weapon_rot = rot + RAND_F(-90, 90) * DEG2RAD;
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
							weapon.AddComponent(transform);
							auto animCom = AnimationCom();
							animCom.Add("iceBallAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							animCom.GetCurrentAnimation()->SetAnimIdle(false);
							weapon.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 0.25));
							weapon.AddComponent(sprite);
							
							auto aabb = BodyCom();
							aabb.ChooseShape("AABB", 0.25, 0.25);
							weapon.AddComponent(aabb);
							weapon.AddComponent(LifeTimeCom(1.0));
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
						}
					}
					num_spawn+=1;
					//std::cerr << 2*num_spawn << '\n';
					{
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::BOLT);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
						auto weapon_rot = rot;
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						auto attach = AttachedMovementCom();
						attach.followPos = true;
						attach.rPos = ToVec(weapon_rot) * 0.5f;
						weapon.AddComponent(attach);
						weapon.AddComponent(transform);
						auto animCom = AnimationCom();
						animCom.Add("boltAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						animCom.GetCurrentAnimation()->SetAnimIdle(false);
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25, 1.0));
						weapon.AddComponent(sprite);
						auto aabb = BodyCom();
						aabb.ChooseShape("AABB", 0.25, 1.0);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(0.5));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
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