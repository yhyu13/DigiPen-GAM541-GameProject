/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/17/2020
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
	class MobPortalControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		std::vector<GameObjectType> enemyTypes;
	public:
		MobPortalControllerComSys() {
			enemyTypes = { GameObjectType::ENEMY };
		}

		virtual void Update(double dt) override {

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto allPortals = m_parentWorld->GetAllEntityWithType(GameObjectType::ENEMY_PORTAL);

			// if exist portal, level is started and is not time out, then spawning enemies
			if (!(!allPortals.empty() && GameLevelMapManager::GetInstance()->IsLevelStarted() && !GameLevelMapManager::GetInstance()->IsTimeOut()))
			{
				return;
			}

			for (auto& portal : allPortals)
			{	
				// Check active
				ComponentDecorator<ActiveCom, GameObjectType> active;
				m_parentWorld->Unpack(portal, active);
				if (!active->IsActive())
				{
					continue;
				}
				ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
				m_parentWorld->Unpack(portal, coolDownController);
				if (coolDownController->IsFreezed())
				{
					continue;
				}
				if (coolDownController->IsCoolDown())
				{
					coolDownController->Update(dt);
					continue;
				}
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(portal, transform);
				auto e = MemoryManager::Make_shared<SpawnEvent>(enemyTypes[RAND_I(0, enemyTypes.size())], transform->GetPos3D());
				queue->Publish(e);
				coolDownController->Update(dt);
			}
		}
	};
}