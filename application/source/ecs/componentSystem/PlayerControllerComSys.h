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

				auto input = InputManager::GetInstance();
				ComponentDecorator<TransformCom, GameObjectType> position;
				ComponentDecorator<AnimationCom, GameObjectType> animation;
				m_parentWorld->Unpack(entity, position);
				m_parentWorld->Unpack(entity, animation);

				
				//Control Sprite KeyPress
				animation->GetCurrentAnimation()->SetAnimIdle(true);

				if (input->IsKeyPressed(GLFW_KEY_W) && input->IsAllKeyNotPressed<int, int, int>(GLFW_KEY_D, GLFW_KEY_S, GLFW_KEY_A)) {
					PRINT("KEY W PRESSED!");

					animation->SetCurrentAnimationState("MoveUp");
					animation->GetCurrentAnimation()->SetAnimIdle(false);
					position->m_x += -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y += cos(glm::radians(0.0f)) * 5 * dt;
					
					return;
				}
				if (input->IsKeyPressed(GLFW_KEY_S)  && input->IsAllKeyNotPressed<int, int, int>(GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W)) {
					PRINT("KEY S PRESSED!");

					animation->SetCurrentAnimationState("MoveDown");
					animation->GetCurrentAnimation()->SetAnimIdle(false);
					position->m_x -= -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= cos(glm::radians(0.0f)) * 5 * dt;
					return;
				}
				if (input->IsKeyPressed(GLFW_KEY_A) && input->IsAllKeyNotPressed<int, int, int>(GLFW_KEY_D, GLFW_KEY_S, GLFW_KEY_W)) {
					PRINT("KEY A PRESSED!");

					animation->SetCurrentAnimationState("MoveLeft");
					animation->GetCurrentAnimation()->SetAnimIdle(false);
					position->m_x -= cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= sin(glm::radians(0.0f)) * 5 * dt;
					return;
				}
				if (input->IsKeyPressed(GLFW_KEY_D) && input->IsAllKeyNotPressed<int, int, int>(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A)) {
					PRINT("KEY D PRESSED!");

					animation->SetCurrentAnimationState("MoveRight");
					animation->GetCurrentAnimation()->SetAnimIdle(false);
					position->m_x += cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y += sin(glm::radians(0.0f)) * 5 * dt;
					return;
				}
			}
		}
	};
}