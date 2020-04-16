/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: PlayerSkillSystem.h
Purpose			: Holds gameplay logic for handling player skills.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/events/EventQueue.h"
#include "engine/math/MathHelper.h"
#include "ecs/EntityType.h"
#include "ecs/EventType.h"
#include "ecs/components/PlayerSkillComponent.h"
#include "ecs/components/ActiveCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/CoolDownCom.h"
#include "ecs/components/ChildrenCom.h"
#include "ecs/components/DamageCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/CustomEvents.h"
#include "skill-manager/SkillManager.h"
#include "skill-system/active-skills/RazorAttack.h"
#include "ecs/components/TargetEntityComponent.h"
#include "skill-system/active-skills/CycloneAttack.h"

#include <sstream>

namespace gswy
{
	class PlayerSkillSystem : public BaseComponentSystem<GameObjectType>
	{
	public:

		PlayerSkillSystem()
		{
			m_systemSignature.AddComponent<PlayerSkillComponent>();
		}

		~PlayerSkillSystem()
		{
		}

		void Init()
		{
			m_spawnZOrder = 5000;

			m_queue = EventQueue<GameObjectType, EventType>::GetInstance();
			m_queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_USE, &PlayerSkillSystem::OnSkillUse);
			m_queue->Subscribe<PlayerSkillSystem>(this, EventType::FORK, &PlayerSkillSystem::OnFork);
		}

		virtual void Update(double dt) override
		{
			if (m_spawnZOrder > 9000)
			{
				m_spawnZOrder = 5000;
			}

			{
				// Update cool down of razer while active
				auto cyclone_sfx = m_parentWorld->GetAllEntityWithType(GameObjectType::CYCLONE_SFX)[0];
				auto active = GetComponent<ActiveCom>(cyclone_sfx);
				auto cooldown = GetComponent<CoolDownCom>(cyclone_sfx);
				if (active->IsActive())
				{
					cooldown->SetFreeze(false);
					cooldown->Update(dt);
				}
				else
				{
					cooldown->SetFreeze(true);
					cooldown->ResetCoolDown();
				}
			}

		}

		void OnSkillUse(EventQueue<GameObjectType, EventType>::EventPtr event)
		{

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto skillUseEvent = std::static_pointer_cast<SkillUseEvent>(event);
			auto fireballAttack = std::dynamic_pointer_cast<FireballAttack>(skillUseEvent->m_skill);
			auto iceballAttack = std::dynamic_pointer_cast<IceballAttack>(skillUseEvent->m_skill);
			auto cycloneAttack = std::dynamic_pointer_cast<CycloneAttack>(skillUseEvent->m_skill);
			auto razorAttack = std::dynamic_pointer_cast<RazorAttack>(skillUseEvent->m_skill);
			ComponentDecorator<TransformCom, GameObjectType> transform;

			auto player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];

			m_parentWorld->Unpack(player, transform);

			if (fireballAttack != nullptr)
			{
				int num_spawn = fireballAttack->GetCount();
				for (int i = 0; i < num_spawn; ++i)
				{
					{
						auto aoe_multipler = fireballAttack->GetAOEMultipler();
						auto damage = fireballAttack->GetDamage();

						auto pos = transform->GetPos();
						auto rot = transform->GetRotation();
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FIREBALL);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(player));
						auto weapon_rot = rot;
						if (num_spawn > 1)
						{
							weapon_rot += RAND_F(-45, 45) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						weapon.AddComponent(transform);
						auto particle = ParticleCom();
						particle.Init<ExplosionParticle>();
						weapon.AddComponent(particle);
						auto animCom = AnimationCom();
						animCom.Add("fireBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25* aoe_multipler, 0.25* aoe_multipler));
						weapon.AddComponent(sprite);
						auto aabb = BodyCom();
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
						aabb.ChooseShape("Circle", 0.1* aoe_multipler);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
						weapon.AddComponent(DamageCom(damage));
					}
				}
				auto e = MemoryManager::Make_shared<WeaponSoundEvent>("fireball_shoot_lr1", transform->GetPos());
				queue->Publish(e);
			}
			else if (iceballAttack != nullptr)
			{
				int num_spawn = iceballAttack->GetCount();
				for (int i = 0; i < num_spawn; ++i)
				{
					{
						auto aoe_multipler = iceballAttack->GetAOEMultipler();
						auto damage = iceballAttack->GetDamage();

						auto pos = transform->GetPos();
						auto rot = transform->GetRotation();
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::ICEBALL);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(player));
						auto weapon_rot = rot;
						if (num_spawn > 1)
						{
							weapon_rot += RAND_F(-45, 45) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						weapon.AddComponent(transform);
						auto animCom = AnimationCom();
						animCom.Add("iceBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25* aoe_multipler, 0.25* aoe_multipler));
						weapon.AddComponent(sprite);

						auto aabb = BodyCom();
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
						aabb.ChooseShape("Circle", 0.1* aoe_multipler);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
						weapon.AddComponent(DamageCom(damage));
					}
				}
				auto e = MemoryManager::Make_shared<WeaponSoundEvent>("ice_shoot1", transform->GetPos());
				queue->Publish(e);
			}
			else if (cycloneAttack != nullptr)
			{
				auto aoe_multipler = cycloneAttack->GetAOEMultipler();
				auto damage = cycloneAttack->GetDamage();
				auto weapon = m_parentWorld->GetAllEntityWithType(GameObjectType::CYCLONE_SFX)[0];
				auto sprite = GetComponent<SpriteCom>(weapon);
				sprite->SetScale(vec2(0.5 * aoe_multipler, 0.5 * aoe_multipler));
				auto body = GetComponent<BodyCom>(weapon);
				body->ChooseShape("Circle", 0.5* aoe_multipler);
				auto damageCom = GetComponent<DamageCom>(weapon);
				damageCom->SetDamange(damage);

				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(player, "CycloneAttack", true);
				queue->Publish(e);

				auto audio = AudioManager::GetInstance();
				if (!audio->IsPlaying("cyclone_shoot"))
				{
					auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("cyclone_shoot", body->GetPos());
					queue->Publish(e1);
				}
			}
			else if (razorAttack != nullptr)
			{
				auto aoe_multipler = razorAttack->GetAOEMultipler();
				auto damage = razorAttack->GetDamage();

				auto pos = transform->GetPos();
				auto rot = transform->GetRotation();

				auto weapon = m_parentWorld->GenerateEntity(GameObjectType::RAZOR);
				auto active = ActiveCom();
				weapon.AddComponent(active);

				weapon.AddComponent(OwnershiptCom<GameObjectType>(player));
				
				auto cooldown = CoolDownCom(0.25);
				weapon.AddComponent(cooldown);

				auto weapon_rot = rot;
				auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
				weapon.AddComponent(transform);

				auto animation = AnimationCom();
				animation.Add("Animation_Razor_Attack", "Move");
				animation.SetCurrentAnimationState("Move");
				weapon.AddComponent(animation);

				auto sprite = SpriteCom();
				sprite.SetScale(vec2(0.25* aoe_multipler, 0.25* aoe_multipler));
				weapon.AddComponent(sprite);

				auto aabb = BodyCom();
				aabb.m_overrideFriction = true;
				aabb.SetPos(transform.GetPos());
				aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
				aabb.ChooseShape("Circle", 0.1* aoe_multipler);
				weapon.AddComponent(aabb);

				weapon.AddComponent(LifeTimeCom(1.5));

				auto targetEntityComponent = TargetEntityComponent();
				weapon.AddComponent(targetEntityComponent);

				weapon.AddComponent(DamageCom(damage));

				auto e = MemoryManager::Make_shared<WeaponSoundEvent>("razor_shoot2", pos);
				queue->Publish(e);
			}
		}

		void OnFork(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			auto forkEvent = std::static_pointer_cast<ForkEvent>(event);
			ActiveSkillType type = forkEvent->m_skillType;
			glm::vec2 position = forkEvent->m_position;
			float rotation = forkEvent->m_rotation;

			auto player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];

			SkillManager* manager = SkillManager::GetInstance();
			std::shared_ptr<ActiveSkill> skill = manager->GetActiveSkill(forkEvent->m_skillType);
			if (skill != nullptr)
			{
				if (auto fireballAttack = std::dynamic_pointer_cast<FireballAttack>(skill))
				{
					int num_spawn = fireballAttack->GetForkCount();
					for (int i = 0; i < num_spawn; ++i)
					{
						{
							auto aoe_multipler = fireballAttack->GetAOEMultipler();
							auto damage = fireballAttack->GetDamage();

							auto pos = position;
							auto rot = rotation;
							auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FORKED_FIREBALL);
							auto active = ActiveCom();
							weapon.AddComponent(active);
							weapon.AddComponent(OwnershiptCom<GameObjectType>(player));
							auto weapon_rot = rot;
							if (num_spawn > 1)
							{
								weapon_rot += RAND_F(-45, 45) * DEG2RAD;
							}
							auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
							weapon.AddComponent(transform);
							auto particle = ParticleCom();
							particle.Init<ExplosionParticle>();
							weapon.AddComponent(particle);
							auto animCom = AnimationCom();
							animCom.Add("fireBallAnim1", "Move");
							animCom.SetCurrentAnimationState("Move");
							weapon.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.15* aoe_multipler, 0.15* aoe_multipler));
							weapon.AddComponent(sprite);
							auto aabb = BodyCom();
							aabb.SetPos(transform.GetPos());
							aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
							aabb.ChooseShape("Circle", 0.1* aoe_multipler);
							weapon.AddComponent(aabb);
							weapon.AddComponent(LifeTimeCom(1.0));
							weapon.AddComponent(HitPreventionCom<GameObjectType>());
							weapon.AddComponent(DamageCom(damage*2/3));
						}
					}
				}
			}
		}

	private:
		int m_spawnZOrder;
		EventQueue<GameObjectType, EventType>* m_queue;
	};
}
