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
#include "ecs/components/DamageCom.h"
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
			
			if (!dt)
			{
				return;
			}
			SyncRegisteredEntities();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			std::vector<Entity<GameObjectType>> deathList;

			// Calculate hp
			for (auto& entity : m_registeredEntities) {
				
				// Check active
				auto active = GetComponent<ActiveCom>(entity);
				if (!active->IsActive())
				{
					continue;
				}

				auto HitPoint = GetComponent<HitPointCom>(entity);

				if (HitPoint->IsDepleted())
				{
					if (!HitPoint->IsDead())
					{
						HitPoint->SetIsDead(true);
						deathList.push_back(entity);
					}
				}
			}

			// Change hp bar
			auto bars = m_parentWorld->GetAllEntityWithType(GameObjectType::HP_BAR);
			for (auto& hp_barEntity : bars)
			{
				// Check active
				auto active = GetComponent<ActiveCom>(hp_barEntity);
				if (!active->IsActive())
				{
					continue;
				}

				auto ownerCom = GetComponent<OwnershiptCom<GameObjectType>>(hp_barEntity);
				// Remove hp bar for dead onwer entity
				if (std::find(deathList.begin(), deathList.end(), ownerCom->GetEntity()) != deathList.end())
				{
					continue;
				}

				// Scale the hp bar on hp changes
				auto ownerHPCom = GetComponent<HitPointCom>(ownerCom->GetEntity());
				auto hp_barSprite = GetComponent<SpriteCom>(hp_barEntity);
				hp_barSprite->SetScale(vec2(ownerHPCom->GetPercentageHP() * 0.20, 0.02));
			}

			// Publish death event
			for (auto& entity : deathList)
			{
				auto e = MemoryManager::Make_shared<DeathEvent>(entity);
				queue->Publish(e);
			}
		}

		void OnCOLLISION(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<CollisionEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				bool isSwasp = false;
				auto entityA = event->m_entityA;
				auto entityB = event->m_entityB;
				auto position = event->m_position;
				auto rotation = event->m_rotation;

				BEGIN:
				switch (entityA.m_type)
				{
				case GameObjectType::PLAYER:
					PLAYER(entityA, entityB);
					goto END;
				case GameObjectType::ENEMY_1: case GameObjectType::ENEMY_2: case GameObjectType::ENEMY_BOSS_1: case GameObjectType::ENEMY_BOSS_2:
					ENEMY(entityA, entityB, position, rotation);
					goto END;
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
				END:
				return;
			}
		}

		void PLAYER(const Entity<GameObjectType>& entityA, const Entity<GameObjectType>& entityB)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			switch (entityB.m_type)
			{
			case GameObjectType::ENEMY_PROJECTILE:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());

					DEBUG_PRINT("Player is hit! HP: " + Str(HitPoint->GetPercentageHP()*100) + "%");

					auto speedDownBuff = MemoryManager::Make_shared<ModifySpeedPercentBuff>(0.33, 0.5);
					auto e = MemoryManager::Make_shared<AddBuffEvent>(entityA, speedDownBuff, true);
					queue->Publish(e);
				}
			}	
				break;
			default:
				break;
			}
		}

		void ENEMY(const Entity<GameObjectType>& entityA, const Entity<GameObjectType>& entityB, const glm::vec2& position, const float& rotation)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			switch (entityB.m_type)
			{
			case GameObjectType::CYCLONE_SFX:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<CoolDownCom, GameObjectType> cooldown;
				ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, cooldown);
				m_parentWorld->Unpack(entityB, owner);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkill;
				m_parentWorld->Unpack(owner->GetEntity(), playerSkill);

				if (!cooldown->IsFreezed() && !cooldown->IsCoolDown())
				{
					// Cyclone deals damage based on percentage of enemy health
					HitPoint->AddHitPoint(-(Damage->GetDamange() + HitPoint->GetMaxHP() * 0.075));
				}
			}
			break;
			case GameObjectType::FIREBALL:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner;
				m_parentWorld->Unpack(entityB, owner);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());

					auto e = MemoryManager::Make_shared<GCEvent>(entityB);
					queue->Publish(e, 0.15);
					

					if (owner->GetEntity().m_type == GameObjectType::PLAYER)
					{
						auto forkEvent = MemoryManager::Make_shared<ForkEvent>(ActiveSkillType::FIRE_BALL, position, rotation);
						queue->Publish(forkEvent);
					}
				}
			}
				break;

			case GameObjectType::FORKED_FIREBALL:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());
				}
			}
			break;

			case GameObjectType::ICEBALL:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				// Note: Iceball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());

					// Iceball makes target it hit slow down
					auto speedDownBuff = MemoryManager::Make_shared<ModifySpeedPercentBuff>(0.5, 1);
					auto e = MemoryManager::Make_shared<AddBuffEvent>(entityA, speedDownBuff, true);
					queue->Publish(e);
				}
			}
			break;

			case GameObjectType::FORKED_ICEBALL:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());

					// Iceball makes target it hit slow down
					auto speedDownBuff = MemoryManager::Make_shared<ModifySpeedPercentBuff>(0.8, 0.5);
					auto e = MemoryManager::Make_shared<AddBuffEvent>(entityA, speedDownBuff, true);
					queue->Publish(e);
				}
			}
			break;

			case GameObjectType::BOLT_STRIKE:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<HitPreventionCom<GameObjectType>, GameObjectType> HitPrevention;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, HitPrevention);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				// Note: Fireball has hit prevention that only applies one hit to enemy
				if (!HitPrevention->IsIncluded(entityA))
				{
					HitPrevention->Add(entityA);
					HitPoint->AddHitPoint(-Damage->GetDamange());
				}
			}
			break;

			case GameObjectType::RAZOR:
			{
				ComponentDecorator<HitPointCom, GameObjectType> HitPoint;
				ComponentDecorator<CoolDownCom, GameObjectType> cooldown;
				ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner;
				m_parentWorld->Unpack(entityA, HitPoint);
				m_parentWorld->Unpack(entityB, cooldown);
				m_parentWorld->Unpack(entityB, owner);

				ComponentDecorator<DamageCom, GameObjectType> Damage;
				m_parentWorld->Unpack(entityB, Damage);

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkill;
				m_parentWorld->Unpack(owner->GetEntity(), playerSkill);

				if (!cooldown->IsFreezed() && !cooldown->IsCoolDown())
				{
					HitPoint->AddHitPoint(-Damage->GetDamange());
				}
			}
			break;

			default:
				break;
			}
		}
	};
}