/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/11/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/input/InputManager.h"
#include "ecs/components/ActiveCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/CoolDownCom.h"
#include "ecs/components/MiniMapSprite.h"
#include "ecs/EntityType.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class TowerControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		int m_towerBuildCost = {100};
		bool m_bCanBuild = { false };
	public:
		TowerControllerComSys() {
		}

		virtual void Init() override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<TowerControllerComSys>(this, EventType::CLICK_ON_TOWER, &TowerControllerComSys::OnClickOnTower);

			Json::Value root;
			std::ifstream file("./asset/archetypes/entities/tower-config.json", std::ifstream::binary);
			file >> root;
			file.close();
			Json::Value items = root["data"];
			m_towerBuildCost = items["cost"].asInt();
		}

		virtual void Update(double dt) override 
		{
			m_registeredEntities = m_parentWorld->GetAllEntityWithType(GameObjectType::TOWER_BUILD);
			m_bCanBuild = GameLevelMapManager::GetInstance()->GetCoins() >= m_towerBuildCost;
			if (!m_bCanBuild)
			{
				for (auto& tower : m_registeredEntities)
				{
					GetComponent<SpriteCom>(tower)->SetTexture("TowerHammer_Off");
				}
			}


			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			std::vector<Entity<GameObjectType>> allEnemies;
			int trails = 5;

			// Get a specific type of enemy as targets
			while (allEnemies.empty() && (--trails > 0))
			{
				allEnemies = m_parentWorld->GetAllEntityWithType(g_enemyTypes[RAND_I(0, g_enemyTypes.size())]);
			}
			if (allEnemies.empty())
			{
				return;
			}
			
			for (auto& towerType : g_towerTypes)
			{
				m_registeredEntities = m_parentWorld->GetAllEntityWithType(towerType);
				for (auto& tower : m_registeredEntities)
				{
					// Check active
					ComponentDecorator<ActiveCom, GameObjectType> active;
					m_parentWorld->Unpack(tower, active);
					if (!active->IsActive())
					{
						continue;
					}
					ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
					m_parentWorld->Unpack(tower, coolDownController);
					if (coolDownController->IsFreezed())
					{
						continue;
					}

					coolDownController->Update(dt);
					if (coolDownController->IsCoolDown())
					{
						continue;
					}
					ComponentDecorator<TransformCom, GameObjectType> transform;
					m_parentWorld->Unpack(tower, transform);
					auto center = transform->GetPos();

					bool find_nearest = false;
					vec2 closest_enmey_delta(5);

					if (towerType == GameObjectType::TOWER_LIGHTNING)
					{
						closest_enmey_delta = vec2(1);
					}
					for (auto& entity : allEnemies) {
						ComponentDecorator<TransformCom, GameObjectType> transform;
						m_parentWorld->Unpack(entity, transform);
						auto enmey_pos = transform->GetPos();
						auto delta = enmey_pos - center;

						// Find the cloest enmey position
						if (glm::length(delta) < glm::length(closest_enmey_delta))
						{
							closest_enmey_delta = delta;
							find_nearest = true;
						}
					}

					if (find_nearest)
					{
						auto e = MemoryManager::Make_shared<FireWeaponEvent>(tower, transform->GetPos(), LookAt(closest_enmey_delta));
						queue->Publish(e);
					}
				}
			}
		}

		void OnClickOnTower(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<ClickOnTowerEvent>(e))
			{
				auto entity = event->m_entity;

				switch (entity.m_type)
				{
				case GameObjectType::TOWER_BUILD:
				{
					// Proceed only when tower can be built
					if (!m_bCanBuild)
					{
						break;
					}

					auto tower = entity;
					ComponentDecorator<SpriteCom, GameObjectType> towerSprite;
					ComponentDecorator<ChildrenCom<GameObjectType>, GameObjectType> towerChildren;
					m_parentWorld->Unpack(tower, towerSprite);
					m_parentWorld->Unpack(tower, towerChildren);

					if (towerSprite->GetTextureName().compare("TowerHammer_On") == 0)
					{
						towerSprite->SetTexture("TowerHammer_Off");
						for (auto& _tower : towerChildren->GetEntities())
						{
							ComponentDecorator<ActiveCom, GameObjectType> active;
							ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
							m_parentWorld->Unpack(_tower, active);
							m_parentWorld->Unpack(_tower, coolDownController);
							active->SetActive(true);
							coolDownController->SetFreeze(true);
						}
					}
					else
					{
						towerSprite->SetTexture("TowerHammer_On");
						for (auto& _tower : towerChildren->GetEntities())
						{
							ComponentDecorator<ActiveCom, GameObjectType> active;
							m_parentWorld->Unpack(_tower, active);
							active->SetActive(false);
						}
					}
				}
				break;
				case GameObjectType::TOWER_FIRE: case GameObjectType::TOWER_ICE: case GameObjectType::TOWER_LIGHTNING:
				{
					auto _tower = entity;
					ComponentDecorator<BodyCom, GameObjectType> transform;
					ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
					ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> ownership;
					m_parentWorld->Unpack(_tower, transform);
					m_parentWorld->Unpack(_tower, coolDownController);
					m_parentWorld->Unpack(_tower, ownership);

					// Unfreeze tower as a way to show it has been enabled.
					// Do not re-enabled unfreezed tower
					if (!coolDownController->IsFreezed())
					{
						break;
					}
					// If tower is still in freezing stat, proceed
					// Do not enable tower if insufficent coins
					if (!GameLevelMapManager::GetInstance()->TrySpendCoins(m_towerBuildCost))
					{
						break;
					}

					coolDownController->SetFreeze(false);
					auto tower = ownership->GetEntity();
					ComponentDecorator<BodyCom, GameObjectType> towerBody;
					ComponentDecorator<ActiveCom, GameObjectType> towerActive;
					ComponentDecorator<ChildrenCom<GameObjectType>, GameObjectType> towerChildren;
					m_parentWorld->Unpack(tower, towerBody);
					m_parentWorld->Unpack(tower, towerActive);
					m_parentWorld->Unpack(tower, towerChildren);

					transform->SetPos(towerBody->GetPos());
					
					auto sprite0 = MiniMapSprite();
					sprite0.SetScale(vec2(0.1, 0.1));
					sprite0.SetTexture("BlueLayer");
					m_parentWorld->AddComponent(_tower, sprite0);

					towerActive->SetActive(false);
					// Deactivate other towers
					for (auto& child : towerChildren->GetEntities())
					{
						if (child.m_type != _tower.m_type)
						{
							ComponentDecorator<ActiveCom, GameObjectType> active;
							m_parentWorld->Unpack(child, active);
							active->SetActive(false);
						}
					}
				}
				break;
				default:
					break;
				}
			}
		}
	};
}