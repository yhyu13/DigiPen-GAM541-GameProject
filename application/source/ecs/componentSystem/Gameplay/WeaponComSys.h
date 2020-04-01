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

		void OnFIREWEAPON(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<FireWeaponEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				auto transform = GetComponent<TransformCom>(event->m_entity);

				switch (event->m_entity.m_type)
				{
				case GameObjectType::ENEMY_2:
				{
					{
						auto pos = event->m_pos;
						auto rot = event->m_rot;
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_PROJECTILE);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
						auto weapon_rot = rot;//+ RAND_F(-90, 90) * DEG2RAD;
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						//transform.AddVelocity(ToVec(weapon_rot) * 5.0f);
						weapon.AddComponent(transform);
						auto animCom = AnimationCom();
						animCom.Add("iceBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25, 0.25));
						weapon.AddComponent(sprite);
						auto aabb = BodyCom();
						aabb.ChooseShape("Circle", 0.1);
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 1.0f);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
				}
				break;
				case GameObjectType::TOWER_FIRE:
				{
					static int num_spawn = 1;
					for (int i = 0; i < num_spawn; ++i)
					{
						{
							auto pos = event->m_pos;
							auto rot = event->m_rot;
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FIREBALL);
							
							// Active com
							auto active = ActiveCom();
							weapon.AddComponent(active);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
							
							// Transformation com
							auto weapon_rot = rot;//+ RAND_F(-90, 90) * DEG2RAD;
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							//transform.AddVelocity(ToVec(weapon_rot) * 5.0f);
							weapon.AddComponent(transform);

							// Particle com
							auto particle = ParticleCom();
							particle.Init<ExplosionParticle>();
							weapon.AddComponent(particle);

							// Animation com
							auto animCom = AnimationCom();
							animCom.Add("fireBallAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							weapon.AddComponent(animCom);

							// Sprite com
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 0.25));
							weapon.AddComponent(sprite);

							// Collision box
							auto aabb = BodyCom();
							aabb.SetPos(transform.GetPos());
							aabb.SetVelocity(ToVec(weapon_rot) * 5.0f);
							aabb.ChooseShape("Circle", 0.1);
							weapon.AddComponent(aabb);

							// Fireball last for 1 sec
							weapon.AddComponent(LifeTimeCom(1.0));

							// Fireball does not hit one target twice
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
						}
					}
				}
				break;
				case GameObjectType::TOWER_ICE:
				{
					static int num_spawn = 1;
					for (int i = 0; i < num_spawn; ++i)
					{
						{
							auto pos = event->m_pos;
							auto rot = event->m_rot;
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::ICEBALL);
							auto active = ActiveCom();
							weapon.AddComponent(active);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
							auto weapon_rot = rot;//+ RAND_F(-90, 90) * DEG2RAD;
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							//transform.AddVelocity(ToVec(weapon_rot) * 5.0f);
							weapon.AddComponent(transform);
							auto animCom = AnimationCom();
							animCom.Add("iceBallAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							weapon.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 0.25));
							weapon.AddComponent(sprite);
							auto aabb = BodyCom();
							aabb.ChooseShape("Circle", 0.1);
							aabb.SetPos(transform.GetPos());
							aabb.SetVelocity(ToVec(weapon_rot) * 5.0f);
							weapon.AddComponent(aabb);
							weapon.AddComponent(LifeTimeCom(1.0));
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
						}
					}
				}
				break;
				case GameObjectType::TOWER_LIGHTNING:
				{
					static int num_spawn = 1;
					for (int i = 0; i < num_spawn; ++i)
					{
						{
							auto pos = event->m_pos;
							auto rot = event->m_rot;
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::BOLT_STRIKE);
							auto active = ActiveCom();
							weapon.AddComponent(active);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(event->m_entity));
							auto weapon_rot = rot;//+ RAND_F(-90, 90) * DEG2RAD;
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							weapon.AddComponent(transform);
							auto attach = AttachedMovementCom();
							attach.followPos = true;
							attach.rPos = ToVec(weapon_rot) * .5f;
							weapon.AddComponent(attach);
							auto animCom = AnimationCom();
							animCom.Add("boltAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							weapon.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 1));
							weapon.AddComponent(sprite);
							auto aabb = BodyCom();
							aabb.SetPos3D(transform.GetPos3D());
							aabb.ChooseShape("AABB", 0.25, 1.0);
							weapon.AddComponent(aabb);
							weapon.AddComponent(LifeTimeCom(0.5));
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
						}
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