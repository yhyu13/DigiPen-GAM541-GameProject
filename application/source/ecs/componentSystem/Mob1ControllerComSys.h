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
#include "ecs/components/TransformCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class Mob1ControllerComSys : public BaseComponentSystem<GameObjectType> {
	public:
		Mob1ControllerComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<AnimationCom>();
		}

		virtual void Update(double dt) override {

			static auto playerEntity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];

			for (auto& entity : m_registeredEntities) {

				if (entity.m_type != GameObjectType::ENEMY)
					continue;

				{
					ComponentDecorator<TransformCom, GameObjectType> position;
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, position);
					m_parentWorld->Unpack(entity, animation);

					ComponentDecorator<TransformCom, GameObjectType> playerPosition;
					m_parentWorld->Unpack(playerEntity, playerPosition);

					// 1. TODO : TileMap with A* algorithm
					auto delta = playerPosition->GetPos() - position->GetPos();
					position->SetRotation(LookAt(delta));
					
					float speed = .5f;
					position->AddXY(delta * speed *(float)dt);
					
					// 2. Movement with keys
					animation->SetCurrentAnimationState("Move");
					animation->GetCurrentAnimation()->SetAnimIdle(false);
				}
			}
		}
	};
}