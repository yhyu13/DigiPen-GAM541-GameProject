/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/12/2020
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
#include "ecs/components/BuffCom.h"
#include "ecs/components/DamageCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/CoolDownCom.h"
#include "ecs/components/ChildrenCom.h"

namespace gswy
{
	class SpawningComSys : public BaseComponentSystem<GameObjectType> {
	private:
		int m_spawnZOrder;
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

		float MobProbSpawnWithESBasedOnProgreesion(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<SpawnEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

				auto prob = 0.25f;
				auto level = GameLevelMapManager::GetInstance()->m_currentLevel;
				auto wave = GameLevelMapManager::GetInstance()->m_currentWave;

				switch (level)
				{
				case 0:
					prob *= 0;
					break;
				case 1:
					prob *= 1 / 2;
					break;
				case 2:
					prob *= 1 / 2;
					break;
				case 3:
					prob *= 1;
					break;
				default:
					break;
				}

				switch (event->m_type)
				{
				case GameObjectType::ENEMY_1:
					break;
				case GameObjectType::ENEMY_2:
					break;
				case GameObjectType::ENEMY_BOSS_1:
					break;
				case GameObjectType::ENEMY_BOSS_2:
					break;
				default:
					break;
				}

				return prob;
			}
		}

		void OnSPAWN(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<SpawnEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				switch (event->m_type)
				{
				case GameObjectType::ENEMY_PORTAL:
					SpawnEnemeyPortal(e);
					break;
				case GameObjectType::ENEMY_1:
					SpawnEnemey1(e);
					break;
				case GameObjectType::ENEMY_2:
					SpawnEnemey2(e);
					break;
				case GameObjectType::ENEMY_BOSS_1:
					SpawnEnemeyBoss1(e);
					break;
				case GameObjectType::ENEMY_BOSS_2:
					SpawnEnemeyBoss2(e);
					break;
				case GameObjectType::TOWER_BUILD:
					SpawnTower(e);
					break;
				default:
					break;
				}
			}
		}

		void SpawnTower(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto event = static_pointer_cast<SpawnEvent>(e);
			auto tower = m_parentWorld->GenerateEntity(GameObjectType::TOWER_BUILD);
			auto active = ActiveCom();
			tower.AddComponent(active);
			tower.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(vec3(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)),0);
			tower.AddComponent(transform);
			auto sprite = SpriteCom();
			sprite.SetTexture("TowerHammer_On");
			sprite.SetScale(vec2(0.25, 0.25));
			tower.AddComponent(sprite);
			auto body = BodyCom();
			body.SetMass(0);
			body.SetPos(transform.GetPos());
			body.ChooseShape("AABB", 0.25, 0.25);
			tower.AddComponent(body);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(vec2(0.1, 0.1));
			sprite0.SetTexture("BlueLayer");
			tower.AddComponent(sprite0);
			auto children = ChildrenCom<GameObjectType>();
			{
				auto _tower = m_parentWorld->GenerateEntity(GameObjectType::TOWER_FIRE);
				auto active = ActiveCom(false);
				_tower.AddComponent(active);
				_tower.AddComponent(OwnershiptCom<GameObjectType>(tower));
				auto angle = 45 * DEG2RAD;
				auto relativeDistance = RadToVec(angle) * .4f;
				auto transform = TransformCom(vec3(event->m_pos.x+relativeDistance.x, event->m_pos.y+relativeDistance.y, Z_ORDER(m_spawnZOrder++)), 0);
				_tower.AddComponent(transform);
				auto sprite = SpriteCom();
				sprite.SetTexture("TowerFire");
				sprite.SetScale(vec2(0.25, 0.25));
				_tower.AddComponent(sprite);
				auto coolDownController = CoolDownCom(0.5);
				coolDownController.SetFreeze(true);
				_tower.AddComponent(coolDownController);
				auto body = BodyCom();
				body.SetMass(0);
				body.SetPos(transform.GetPos());
				body.ChooseShape("AABB", 0.25, 0.25);
				_tower.AddComponent(body);
				children.AddEntity(_tower);
			}
			{
				auto _tower = m_parentWorld->GenerateEntity(GameObjectType::TOWER_ICE);
				auto active = ActiveCom(false);
				_tower.AddComponent(active);
				_tower.AddComponent(OwnershiptCom<GameObjectType>(tower));
				auto angle = 90 * DEG2RAD;
				auto relativeDistance = RadToVec(angle) * .4f;
				auto transform = TransformCom(vec3(event->m_pos.x + relativeDistance.x, event->m_pos.y + relativeDistance.y, Z_ORDER(m_spawnZOrder++)), 0);
				_tower.AddComponent(transform);
				auto sprite = SpriteCom();
				sprite.SetTexture("TowerIce");
				sprite.SetScale(vec2(0.25, 0.25));
				_tower.AddComponent(sprite);
				auto coolDownController = CoolDownCom(1.0);
				_tower.AddComponent(coolDownController);
				auto body = BodyCom();
				body.SetPos(transform.GetPos());
				body.SetMass(0);
				body.ChooseShape("AABB", 0.25,0.25);
				_tower.AddComponent(body);
				children.AddEntity(_tower);
			}
			{
				auto _tower = m_parentWorld->GenerateEntity(GameObjectType::TOWER_LIGHTNING);
				auto active = ActiveCom(false);
				_tower.AddComponent(active);
				_tower.AddComponent(OwnershiptCom<GameObjectType>(tower));
				auto angle = 135 * DEG2RAD;
				auto relativeDistance = RadToVec(angle) * .4f;
				auto transform = TransformCom(vec3(event->m_pos.x + relativeDistance.x, event->m_pos.y + relativeDistance.y, Z_ORDER(m_spawnZOrder++)), 0);
				_tower.AddComponent(transform);
				auto sprite = SpriteCom();
				sprite.SetTexture("TowerLightning");
				sprite.SetScale(vec2(0.25, 0.25));
				_tower.AddComponent(sprite);
				auto coolDownController = CoolDownCom(0.5);
				_tower.AddComponent(coolDownController);
				auto body = BodyCom();
				body.SetMass(0);
				body.SetPos(transform.GetPos());
				body.ChooseShape("AABB", 0.25, 0.25);
				_tower.AddComponent(body);
				children.AddEntity(_tower);
			}
			tower.AddComponent(children);
		}

		void SpawnEnemeyPortal(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_PORTAL);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
			auto cooldown = CoolDownCom(RAND_F(0.75,1.25));
			obj.AddComponent(cooldown);
			auto animCom = AnimationCom();
			animCom.Add("PortalAnimation_0", "Move");
			animCom.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(0.4, 0.4));
			obj.AddComponent(sprite);
		}

		void SpawnEnemey1(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto spriteScale = vec2(0.5, 0.5);
			auto miniMapScale = vec2(0.1, 0.1);
			auto bbScale = vec2(0.5, 0.5);
			auto energyShieldScale = vec2(0.3, 0.3);

			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_1);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(BuffCom());
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++));
			obj.AddComponent(transform);
			auto animCom2 = AnimationCom();
			animCom2.Add("Mob1Animation_Moving", "Move");
			animCom2.Add("Mob1Animation_Attack", "Attack");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(spriteScale.x, spriteScale.y / 70 * 50));
			obj.AddComponent(sprite);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(miniMapScale);
			sprite0.SetTexture("RedLayer");
			obj.AddComponent(sprite0);
			auto aabb1 = BodyCom();
			aabb1.SetPos(transform.GetPos());
			aabb1.ChooseShape("AABB", bbScale.x, bbScale.y / 70 * 50);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom(100));
			auto cooldown = CoolDownCom(0.1);
			obj.AddComponent(cooldown);
			obj.AddComponent(DamageCom(5));

			auto childrenCom = ChildrenCom<GameObjectType>();

			auto buffCom = BuffCom();
			if (RAND_F(0, 1) < MobProbSpawnWithESBasedOnProgreesion(e))
			{
				auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.25, -1);
				buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
				// Mob energy shield
				{
					auto energy_shield = m_parentWorld->GenerateEntity(GameObjectType::ENERGY_SHIELD);
					auto active = ActiveCom();
					energy_shield.AddComponent(active);
					energy_shield.AddComponent(OwnershiptCom<GameObjectType>(obj));
					auto attach = AttachedMovementCom();
					attach.followPos = true;
					attach.rPos = vec2(0, 0);
					energy_shield.AddComponent(attach);
					energy_shield.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
					energy_shield.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
					auto sprite = SpriteCom();
					sprite.SetScale(energyShieldScale);
					energy_shield.AddComponent(sprite);
					auto animCom2 = AnimationCom();
					animCom2.Add("Energy_shieldAnimation", "Move");
					animCom2.SetCurrentAnimationState("Move");
					energy_shield.AddComponent(animCom2);
					// Add to obj
					childrenCom.AddEntity(energy_shield);
				}
			}
			obj.AddComponent(buffCom);

			// Mob floating hp bar
			{
				auto hp_bar = m_parentWorld->GenerateEntity(GameObjectType::HP_BAR);
				auto active = ActiveCom();
				hp_bar.AddComponent(active);
				hp_bar.AddComponent(OwnershiptCom<GameObjectType>(obj));
				auto attach = AttachedMovementCom();
				attach.followPos = true;
				attach.rPos = vec2(0, 0.2);
				hp_bar.AddComponent(attach);
				hp_bar.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
				hp_bar.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
				auto sprite = SpriteCom();
				sprite.SetTexture("RedLayer");
				sprite.SetScale(vec2(0.20, 0.02));
				hp_bar.AddComponent(sprite);
				// Add to obj
				childrenCom.AddEntity(hp_bar);
			}
			obj.AddComponent(childrenCom);
		}

		void SpawnEnemey2(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto spriteScale = vec2(0.5, 0.5);
			auto miniMapScale = vec2(0.1, 0.1);
			auto bbScale = vec2(0.25, 0.25);
			auto energyShieldScale = vec2(0.3, 0.3);

			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_2);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(BuffCom());
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++));
			obj.AddComponent(transform);
			auto animCom2 = AnimationCom();
			animCom2.Add("Mob2Animation_Moving", "Move");
			animCom2.Add("Mob2Animation_Attack", "Attack");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(spriteScale.x, spriteScale.y / 70 * 50));
			obj.AddComponent(sprite);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(miniMapScale);
			sprite0.SetTexture("RedLayer");
			obj.AddComponent(sprite0);
			auto aabb1 = BodyCom();
			aabb1.SetPos(transform.GetPos());
			aabb1.ChooseShape("AABB", bbScale.x, bbScale.y / 70 * 50);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom(75));
			auto cooldown = CoolDownCom(0.1);
			obj.AddComponent(cooldown);
			obj.AddComponent(DamageCom(5));

			auto childrenCom = ChildrenCom<GameObjectType>();

			auto buffCom = BuffCom();
			if (RAND_F(0, 1) < MobProbSpawnWithESBasedOnProgreesion(e))
			{
				auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.25, -1);
				buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
				// Mob energy shield
				{
					auto energy_shield = m_parentWorld->GenerateEntity(GameObjectType::ENERGY_SHIELD);
					auto active = ActiveCom();
					energy_shield.AddComponent(active);
					energy_shield.AddComponent(OwnershiptCom<GameObjectType>(obj));
					auto attach = AttachedMovementCom();
					attach.followPos = true;
					attach.rPos = vec2(0, 0);
					energy_shield.AddComponent(attach);
					energy_shield.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
					energy_shield.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
					auto sprite = SpriteCom();
					sprite.SetScale(energyShieldScale);
					energy_shield.AddComponent(sprite);
					auto animCom2 = AnimationCom();
					animCom2.Add("Energy_shieldAnimation", "Move");
					animCom2.SetCurrentAnimationState("Move");
					energy_shield.AddComponent(animCom2);
					// Add to obj
					childrenCom.AddEntity(energy_shield);
				}
			}
			obj.AddComponent(buffCom);

			// Mob floating hp bar
			{
				auto hp_bar = m_parentWorld->GenerateEntity(GameObjectType::HP_BAR);
				auto active = ActiveCom();
				hp_bar.AddComponent(active);
				hp_bar.AddComponent(OwnershiptCom<GameObjectType>(obj));
				auto attach = AttachedMovementCom();
				attach.followPos = true;
				attach.rPos = vec2(0, 0.2);
				hp_bar.AddComponent(attach);
				hp_bar.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
				hp_bar.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
				auto sprite = SpriteCom();
				sprite.SetTexture("RedLayer");
				sprite.SetScale(vec2(0.20, 0.02));
				hp_bar.AddComponent(sprite);
				// Add to obj
				childrenCom.AddEntity(hp_bar);
			}
			obj.AddComponent(childrenCom);
		}

		void SpawnEnemeyBoss1(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto spriteScale = vec2(0.5, 0.5);
			auto miniMapScale = vec2(0.2, 0.2);
			auto bbScale = vec2(0.5, 0.5);
			auto energyShieldScale = vec2(0.4, 0.4);

			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_BOSS_1);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++));
			obj.AddComponent(transform);
			auto animCom2 = AnimationCom();
			animCom2.Add("Mob_Boss1Animation_Moving", "Move");
			animCom2.Add("Mob_Boss1Animation_Attack", "Attack");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(spriteScale);
			obj.AddComponent(sprite);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(miniMapScale);
			sprite0.SetTexture("RedLayer");
			obj.AddComponent(sprite0);
			auto aabb1 = BodyCom();
			aabb1.SetPos(transform.GetPos());
			aabb1.ChooseShape("AABB", bbScale.x, bbScale.y);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom(200));
			auto cooldown = CoolDownCom(0.1);
			obj.AddComponent(cooldown);
			obj.AddComponent(DamageCom(20));

			auto childrenCom = ChildrenCom<GameObjectType>();

			auto buffCom = BuffCom();
			if (RAND_F(0, 1) < MobProbSpawnWithESBasedOnProgreesion(e))
			{
				auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.25, -1);
				buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
				// Mob energy shield
				{
					auto energy_shield = m_parentWorld->GenerateEntity(GameObjectType::ENERGY_SHIELD);
					auto active = ActiveCom();
					energy_shield.AddComponent(active);
					energy_shield.AddComponent(OwnershiptCom<GameObjectType>(obj));
					auto attach = AttachedMovementCom();
					attach.followPos = true;
					attach.rPos = vec2(0, 0);
					energy_shield.AddComponent(attach);
					energy_shield.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
					energy_shield.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
					auto sprite = SpriteCom();
					sprite.SetScale(energyShieldScale);
					energy_shield.AddComponent(sprite);
					auto animCom2 = AnimationCom();
					animCom2.Add("Energy_shieldAnimation", "Move");
					animCom2.SetCurrentAnimationState("Move");
					energy_shield.AddComponent(animCom2);
					// Add to obj
					childrenCom.AddEntity(energy_shield);
				}
			}
			obj.AddComponent(buffCom);

			// Mob floating hp bar
			{
				auto hp_bar = m_parentWorld->GenerateEntity(GameObjectType::HP_BAR);
				auto active = ActiveCom();
				hp_bar.AddComponent(active);
				hp_bar.AddComponent(OwnershiptCom<GameObjectType>(obj));
				auto attach = AttachedMovementCom();
				attach.followPos = true;
				attach.rPos = vec2(0, 0.2);
				hp_bar.AddComponent(attach);
				hp_bar.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
				hp_bar.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
				auto sprite = SpriteCom();
				sprite.SetTexture("RedLayer");
				sprite.SetScale(vec2(0.20, 0.02));
				hp_bar.AddComponent(sprite);
				// Add to obj
				childrenCom.AddEntity(hp_bar);
			}
			obj.AddComponent(childrenCom);
		}

		void SpawnEnemeyBoss2(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto spriteScale = vec2(0.7, 0.7);
			auto miniMapScale = vec2(0.3, 0.3);
			auto bbScale = vec2(0.7, 0.7);
			auto energyShieldScale = vec2(0.5, 0.5);

			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY_BOSS_2);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++));
			obj.AddComponent(transform);
			auto animCom2 = AnimationCom();
			animCom2.Add("Mob_Boss2Animation_Moving", "Move");
			animCom2.Add("Mob_Boss2Animation_Attack", "Attack");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(spriteScale);
			obj.AddComponent(sprite);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(miniMapScale);
			sprite0.SetTexture("RedLayer");
			obj.AddComponent(sprite0);
			auto aabb1 = BodyCom();
			aabb1.SetPos(transform.GetPos());
			aabb1.ChooseShape("AABB", bbScale.x, bbScale.y);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom(500));
			auto cooldown = CoolDownCom(0.1);
			obj.AddComponent(cooldown);
			obj.AddComponent(DamageCom(30));

			auto childrenCom = ChildrenCom<GameObjectType>();

			auto buffCom = BuffCom();
			if (RAND_F(0, 1) < MobProbSpawnWithESBasedOnProgreesion(e))
			{
				auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.25, -1);
				buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
				// Mob energy shield
				{
					auto energy_shield = m_parentWorld->GenerateEntity(GameObjectType::ENERGY_SHIELD);
					auto active = ActiveCom();
					energy_shield.AddComponent(active);
					energy_shield.AddComponent(OwnershiptCom<GameObjectType>(obj));
					auto attach = AttachedMovementCom();
					attach.followPos = true;
					attach.rPos = vec2(0, 0);
					energy_shield.AddComponent(attach);
					energy_shield.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
					energy_shield.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
					auto sprite = SpriteCom();
					sprite.SetScale(energyShieldScale);
					energy_shield.AddComponent(sprite);
					auto animCom2 = AnimationCom();
					animCom2.Add("Energy_shieldAnimation", "Move");
					animCom2.SetCurrentAnimationState("Move");
					energy_shield.AddComponent(animCom2);
					// Add to obj
					childrenCom.AddEntity(energy_shield);
				}
			}
			obj.AddComponent(buffCom);

			// Mob floating hp bar
			{
				auto hp_bar = m_parentWorld->GenerateEntity(GameObjectType::HP_BAR);
				auto active = ActiveCom();
				hp_bar.AddComponent(active);
				hp_bar.AddComponent(OwnershiptCom<GameObjectType>(obj));
				auto attach = AttachedMovementCom();
				attach.followPos = true;
				attach.rPos = vec2(0, 0.2);
				hp_bar.AddComponent(attach);
				hp_bar.AddComponent(BodyCom(event->m_pos.x, event->m_pos.y));
				hp_bar.AddComponent(TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++)));
				auto sprite = SpriteCom();
				sprite.SetTexture("RedLayer");
				sprite.SetScale(vec2(0.20, 0.02));
				hp_bar.AddComponent(sprite);
				// Add to obj
				childrenCom.AddEntity(hp_bar);
			}

			obj.AddComponent(childrenCom);
		}
	};
}