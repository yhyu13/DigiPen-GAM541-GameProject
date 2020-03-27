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
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/CustomEvents.h"
#include "skill-manager/SkillManager.h"
#include "skill-system/active-skills/RazerAttack.h"

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

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_USE, &PlayerSkillSystem::OnSkillUse);
			queue->Subscribe<PlayerSkillSystem>(this, EventType::FORK, &PlayerSkillSystem::OnFork);
		}

		virtual void Update(double dt) override
		{
			if (m_spawnZOrder > 9000)
			{
				m_spawnZOrder = 5000;
			}
		}

		void OnSkillUse(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			// TODO: handle things like fire-weapon

			APP_DEBUG("Skill Used: {0}", event->m_type);

			auto skillUseEvent = std::static_pointer_cast<SkillUseEvent>(event);
			auto fireballAttack = std::dynamic_pointer_cast<FireballAttack>(skillUseEvent->m_skill);
			auto iceballAttack = std::dynamic_pointer_cast<IceballAttack>(skillUseEvent->m_skill);
			auto razerAttack = std::dynamic_pointer_cast<RazerAttack>(skillUseEvent->m_skill);
			ComponentDecorator<TransformCom, GameObjectType> transform;

			if (m_player == nullptr)
			{
				m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
			}
			m_parentWorld->Unpack(*m_player, transform);

			if (fireballAttack != nullptr) {
				int num_spawn = fireballAttack->GetCount();
				for (int i = 0; i < num_spawn; ++i)
				{
					{
						auto pos = transform->GetPos();
						auto rot = transform->GetRotation();
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FIREBALL);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(*m_player));
						auto weapon_rot = rot;
						if (num_spawn > 1)
						{
							weapon_rot += RAND_F(-45, 45) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						//transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
						weapon.AddComponent(transform);
						auto particle = ParticleCom();
						particle.Init<ExplosionParticle>();
						weapon.AddComponent(particle);
						auto animCom = AnimationCom();
						animCom.Add("fireBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25, 0.25));
						weapon.AddComponent(sprite);
						auto aabb = BodyCom();
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
						aabb.ChooseShape("Circle", 0.1);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
				}
			}
			else if(iceballAttack != nullptr)
			{
				int num_spawn = iceballAttack->GetCount();
				for (int i = 0; i < num_spawn; ++i)
				{
					{
						auto pos = transform->GetPos();
						auto rot = transform->GetRotation();
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::ICEBALL);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(*m_player));
						auto weapon_rot = rot;
						if (num_spawn > 1)
						{
							weapon_rot += RAND_F(-45, 45) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						//transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
						weapon.AddComponent(transform);
						auto animCom = AnimationCom();
						animCom.Add("iceBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.25, 0.25));
						weapon.AddComponent(sprite);

						auto aabb = BodyCom();
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
						aabb.ChooseShape("Circle", 0.1);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
				}
			}
			else if (razerAttack !=  nullptr)
			{
				ComponentDecorator<AnimationCom, GameObjectType> animation;
				m_parentWorld->Unpack(*m_player, animation);
				auto animCom = AnimationCom();
				animCom.Add("RazerAttack1", "RazerAttack");
				animation->SetCurrentAnimationState("RazerAttack");
			}

		}

	void OnFork(EventQueue<GameObjectType, EventType>::EventPtr event)
	{
		auto forkEvent = std::static_pointer_cast<ForkEvent>(event);
		ActiveSkillType type = forkEvent->m_skillType;
		glm::vec2 position = forkEvent->m_position;
		float rotation = forkEvent->m_rotation;

		APP_DEBUG("Fork Event Received: {0}", forkEvent->m_type);

		if (m_player == nullptr)
		{
			m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
		}

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
						auto pos = position;
						auto rot = rotation;
						auto weapon = m_parentWorld->GenerateEntity(GameObjectType::FORKED_FIREBALL);
						auto active = ActiveCom();
						weapon.AddComponent(active);
						weapon.AddComponent(OwnershiptCom<GameObjectType>(*m_player));
						auto weapon_rot = rot;
						if (num_spawn > 1)
						{
							weapon_rot += RAND_F(-45, 45) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						//transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
						weapon.AddComponent(transform);
						auto particle = ParticleCom();
						particle.Init<ExplosionParticle>();
						weapon.AddComponent(particle);
						auto animCom = AnimationCom();
						animCom.Add("fireBallAnim1", "Move");
						animCom.SetCurrentAnimationState("Move");
						weapon.AddComponent(animCom);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.15, 0.15));
						weapon.AddComponent(sprite);
						auto aabb = BodyCom();
						aabb.SetPos(transform.GetPos());
						aabb.SetVelocity(ToVec(weapon_rot) * 2.0f);
						aabb.ChooseShape("Circle", 0.1);
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
				}
			}
		}

		APP_DEBUG("Fork Event PROCESSED: {0}", forkEvent->m_type);
	}

	private:
		std::shared_ptr<Entity<GameObjectType>> m_player;
		int m_spawnZOrder;
	};
}
