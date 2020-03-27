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
#include "ecs/EntityType.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class TowerControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		std::vector<GameObjectType> towerTypes;
		std::vector<GameObjectType> enemyTypes;
	public:
		TowerControllerComSys() {
			towerTypes = { GameObjectType::TOWER_FIRE ,GameObjectType::TOWER_ICE ,GameObjectType::TOWER_LIGHTNING };
			enemyTypes = { GameObjectType::ENEMY_1, GameObjectType::ENEMY_2, GameObjectType::ENEMY_BOSS_1 };
		}

		virtual void Update(double dt) override {

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto allEnemies = m_parentWorld->GetAllEntityWithType(enemyTypes[RAND_I(0, enemyTypes.size())]);
			for (auto& towerType : towerTypes)
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
	};
}