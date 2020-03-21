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
				case GameObjectType::ENEMY:
					SpawnEnemey(e);
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
				auto coolDownController = CoolDownCom(1.0);
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
				auto coolDownController = CoolDownCom(1.0);
				_tower.AddComponent(coolDownController);
				auto body = BodyCom();
				body.SetMass(0);
				body.SetPos(vec3(transform.GetPos(), Z_ORDER(m_spawnZOrder++)));
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
			auto cooldown = CoolDownCom(RAND_F(0.5,1.0));
			obj.AddComponent(cooldown);
			auto animCom = AnimationCom();
			animCom.Add("PortalAnimation_0", "Move");
			animCom.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(0.4, 0.4));
			obj.AddComponent(sprite);
		}

		void SpawnEnemey(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto event = static_pointer_cast<SpawnEvent>(e);
			auto obj = m_parentWorld->GenerateEntity(GameObjectType::ENEMY);
			obj.AddComponent(ActiveCom());
			obj.AddComponent(OwnershiptCom<GameObjectType>());
			auto transform = TransformCom(event->m_pos.x, event->m_pos.y, Z_ORDER(m_spawnZOrder++));
			obj.AddComponent(transform);
			auto animCom2 = AnimationCom();
			animCom2.Add("Mob1Animation_Moving", "Move");
			animCom2.Add("Mob1Animation_Attack", "Attack");
			animCom2.SetCurrentAnimationState("Move");
			obj.AddComponent(animCom2);
			auto sprite = SpriteCom();
			sprite.SetScale(vec2(0.5, 0.5 / 70 * 50));
			obj.AddComponent(sprite);
			auto sprite0 = MiniMapSprite();
			sprite0.SetScale(vec2(0.1, 0.1));
			sprite0.SetTexture("RedLayer");
			obj.AddComponent(sprite0);
			auto aabb1 = BodyCom();
			aabb1.SetPos3D(transform.GetPos3D());
			aabb1.ChooseShape("AABB", 0.5, 0.5 / 70 * 50);
			obj.AddComponent(aabb1);
			obj.AddComponent(HitPointCom());
		}
	};
}