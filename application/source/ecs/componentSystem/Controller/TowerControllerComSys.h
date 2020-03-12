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
#include "ecs/components/TransformCom.h"
#include "ecs/components/FireControllerCom.h"
#include "ecs/EntityType.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class TowerControllerComSys : public BaseComponentSystem<GameObjectType> {
	
	public:
		TowerControllerComSys() {
		}

		virtual void Update(double dt) override {

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto allEnemies = m_parentWorld->GetAllEntityWithType(GameObjectType::ENEMY);
			m_registeredEntities = m_parentWorld->GetAllEntityWithType(GameObjectType::TOWER_FIRE);
			for (auto& tower : m_registeredEntities)
			{
				ComponentDecorator<FireControllerCom, GameObjectType> fireController;
				m_parentWorld->Unpack(tower, fireController);

				if (fireController->isCoolDown())
				{
					fireController->Update(dt);
					continue;
				}

				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(tower, transform);
				auto center = transform->GetPos();

				bool find_nearest = false;
				vec2 closest_enmey_delta(5);
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
				fireController->Update(dt);
			}

		}
	};
}