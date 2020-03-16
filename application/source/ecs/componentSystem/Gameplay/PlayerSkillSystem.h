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
			//m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			//m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
			m_spawnZOrder = 5000;

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_ACTIVATION, &PlayerSkillSystem::OnSkillActivation);
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_UPGRADE, &PlayerSkillSystem::OnSkillUpgrade);
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_USE, &PlayerSkillSystem::OnSkillUse);
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_ADDITION, &PlayerSkillSystem::OnAddSkill);
			queue->Subscribe<PlayerSkillSystem>(this, EventType::SKILL_REMOVAL, &PlayerSkillSystem::OnRemoveSkill);
		}

		virtual void Update(double dt) override
		{
			if (m_spawnZOrder > 9000)
			{
				m_spawnZOrder = 5000;
			}
		}

		void OnAddSkill(EventQueue<GameObjectType, EventType>::EventPtr event /*std::shared_ptr<ActiveSkill> skill*/)
		{
			//if (m_player == nullptr) {
			//	m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			//}
			if (m_player == nullptr)
			{
				m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
			}

			APP_DEBUG("Skill Added: {0}", event->m_type);
			ComponentDecorator<PlayerSkillComponent, GameObjectType> skillComponent;
			m_parentWorld->Unpack(*m_player, skillComponent);

			auto skillAddEvent = std::static_pointer_cast<SkillAdditionEvent>(event);
			skillComponent->AddSkill(skillAddEvent->m_skill);
		}

		void OnRemoveSkill(EventQueue<GameObjectType, EventType>::EventPtr event /*const int& id*/)
		{
			APP_DEBUG("Skill Remove: {0}", event->m_type);
			//if (m_player == nullptr)
			//{
			//	m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			//}
			//Entity<GameObjectType> player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			/*ComponentDecorator<PlayerSkillComponent, GameObjectType> skillComponent;
			m_parentWorld->Unpack(*m_player, skillComponent);
			skillComponent->RemoveSkill(id);*/
		}

		void OnSkillActivation(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			// TODO: do stuff like switching between fire-ball and ice-ball
			//std::shared_ptr<SkillActivationEvent> skillUpgradeEvent = std::static_pointer_cast<SkillActivationEvent>(event);
			if (m_player == nullptr)
			{
				m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
			}

			ComponentDecorator<PlayerSkillComponent, GameObjectType> skillComponent;
			m_parentWorld->Unpack(*m_player, skillComponent);
			skillComponent->SwitchCurrentSkill();
		}

		void OnSkillUpgrade(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			// TODO: do stuff like updating the parameters of fireballs
			// Event must have support skill and active skill information
			APP_DEBUG("Skill Upgraded: {0}", event->m_type);
			std::shared_ptr<SkillUpgradeEvent> skillUpgradeEvent = std::static_pointer_cast<SkillUpgradeEvent>(event);
			if (m_player == nullptr)
			{
				m_player = MemoryManager::Make_shared<Entity<GameObjectType>>(m_registeredEntities.at(0));
			}
			
			ComponentDecorator<PlayerSkillComponent, GameObjectType> skillComponent;
			m_parentWorld->Unpack(*m_player, skillComponent);
			skillComponent->UpgradeSkill(skillUpgradeEvent->m_activeSkillType, skillUpgradeEvent->m_supportSkill);
		}

		void OnSkillUse(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			// TODO: handle things like fire-weapon

			APP_DEBUG("Skill Used: {0}", event->m_type);

			auto skillUseEvent = std::static_pointer_cast<SkillUseEvent>(event);
			auto fireballAttack = std::dynamic_pointer_cast<FireballAttack>(skillUseEvent->m_skill);
			auto iceballAttack = std::dynamic_pointer_cast<IceballAttack>(skillUseEvent->m_skill);
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
							weapon_rot += RAND_F(-90, 90) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
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
							weapon_rot += RAND_F(-90, 90) * DEG2RAD;
						}
						auto transform = TransformCom(vec3(pos.x, pos.y, Z_ORDER(m_spawnZOrder++)), weapon_rot);
						transform.AddVelocity(ToVec(weapon_rot) * 2.0f);
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
						weapon.AddComponent(aabb);
						weapon.AddComponent(LifeTimeCom(1.0));
						weapon.AddComponent(HitPreventionCom<GameObjectType>());
					}
				}
			}

		}

	private:
		std::shared_ptr<Entity<GameObjectType>> m_player;
		int m_spawnZOrder;
	};
}
