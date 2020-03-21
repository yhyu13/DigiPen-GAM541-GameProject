/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/16/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/input/InputManager.h"
#include "engine/ai/PathFinding.h"
#include "tilemap/GameLevelMapManager.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class Mob1ControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		double m_updateTimer = {1.0/10.0};
	public:
		Mob1ControllerComSys() {
		}

		virtual void Update(double dt) override {

			static double timer = m_updateTimer;
			timer += dt;
			if (timer < m_updateTimer)
			{
				return;
			}
			timer = 0.0;

			auto tileMapObj = GameLevelMapManager::GetInstance()->GetCurrentMap();
			auto pathGrid = tileMapObj->GetTileGrid("MobPath");
			auto Astar = tileMapObj->GetPathFinder("MobPath");
			auto destEntity = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
			m_registeredEntities = m_parentWorld->GetAllEntityWithType(GameObjectType::ENEMY);
			for (auto& entity : m_registeredEntities) {
				{
					ComponentDecorator<TransformCom, GameObjectType> transform;
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, transform);
					m_parentWorld->Unpack(entity, animation);

					ComponentDecorator<TransformCom, GameObjectType> destPosition;
					m_parentWorld->Unpack(destEntity, destPosition);

					auto dest = destPosition->GetPos();
					auto src = transform->GetPos();

					auto delta = dest - src;

					// Stop when delta distance is small
					if (glm::length(delta) < .2)
					{
						transform->SetVelocity(vec2(0));
						animation->SetCurrentAnimationState("Attack");
						continue;
					}

					auto _dest = tileMapObj->World2Grid(dest);
					auto _src = tileMapObj->World2Grid(src);
					
					if (Astar->Search(*pathGrid, _src, _dest))
					{
						auto result = Astar->GetResult();
						// 1. Rotate
						auto nextPos = tileMapObj->Grid2World((result.size() > 3)? result[3]: result.back());
						auto delta = nextPos - src;
						transform->SetRotation(LookAt(delta));

						// 2. Move
						float speed = .25f;
						transform->SetVelocity(glm::normalize(delta) * speed);
						animation->SetCurrentAnimationState("Move");
					}
					else
					{
						DEBUG_PRINT(Str(entity) + " not found");
					}
				}
			}
		}
	};
}