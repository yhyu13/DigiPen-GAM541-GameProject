/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once

#include <GLFW/glfw3.h>
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
	class PlayerControllerComSys : public BaseComponentSystem<GameObjectType> {
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<AnimationCom>();
		}

		virtual void Update(double dt) override {
			for (auto& entity : m_registeredEntities) {

				if (entity.m_type != PLAYER)
					continue;

				{
					auto input = InputManager::GetInstance();
					ComponentDecorator<TransformCom, GameObjectType> position;
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, position);
					m_parentWorld->Unpack(entity, animation);

					// 1. Making player facing the cursor
					auto cursor = vec2(input->GetMousePositionX(), input->GetMousePositionY());
					auto center = vec2(input->GetMouseMaxPositionX() / 2, input->GetMouseMaxPositionY() / 2);
					auto delta = cursor - center;
					position->SetRotation((delta.y) < 0? atanf(delta.x / (delta.y+1e-4f)) : 3.1415926f + atanf(delta.x / (delta.y + 1e-4f)));
					DEBUG_PRINT(position->GetRotation());

					// 2. Movement with keys
					bool isIdle = true;
					if (input->IsKeyPressed(GLFW_KEY_W) && input->IsAllKeyNotPressed<int>(GLFW_KEY_S)) {
						PRINT("KEY W PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						position->AddXY(vec2(sinf(glm::radians(0.0f)) * 5 * dt, cosf(glm::radians(0.0f)) * 5 * dt));
					}
					if (input->IsKeyPressed(GLFW_KEY_S) && input->IsAllKeyNotPressed<int>(GLFW_KEY_W)) {
						PRINT("KEY S PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						position->AddXY(vec2(sinf(glm::radians(0.0f)) * 5 * dt, -cosf(glm::radians(0.0f)) * 5 * dt));
					}
					if (input->IsKeyPressed(GLFW_KEY_A) && input->IsAllKeyNotPressed<int>(GLFW_KEY_D)) {
						PRINT("KEY A PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						position->AddXY(vec2(-cosf(glm::radians(0.0f)) * 5 * dt, sinf(glm::radians(0.0f)) * 5 * dt));
					}
					if (input->IsKeyPressed(GLFW_KEY_D) && input->IsAllKeyNotPressed<int>(GLFW_KEY_A)) {
						PRINT("KEY D PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						position->AddXY(vec2(cosf(glm::radians(0.0f)) * 5 * dt, sinf(glm::radians(0.0f)) * 5 * dt));
					}
					animation->GetCurrentAnimation()->SetAnimIdle(isIdle);
					return;
				}
			}
		}
	};
}