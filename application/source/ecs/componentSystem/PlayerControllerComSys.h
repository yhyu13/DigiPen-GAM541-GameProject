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
#include "engine/input/Input.h"
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

				auto input = Input::GetInstance();
				ComponentDecorator<TransformCom, GameObjectType> position;
				ComponentDecorator<AnimationCom, GameObjectType> animation;
				m_parentWorld->Unpack(entity, position);
				m_parentWorld->Unpack(entity, animation);
				//Control Sprite Trigger
				if (input->IsKeyTriggered(GLFW_KEY_W)) {
					PRINT("KEY W TRIGGERED!");
					animation->setCurrentAnimationState("MoveUp");
				}
				else if (input->IsKeyTriggered(GLFW_KEY_S)) {
					PRINT("KEY S TRIGGERED!");
					animation->setCurrentAnimationState("MoveDown");
				}
				else if (input->IsKeyTriggered(GLFW_KEY_A)) {
					PRINT("KEY A TRIGGERED!");
					animation->setCurrentAnimationState("MoveLeft");
				}
				else if (input->IsKeyTriggered(GLFW_KEY_D)) {
					PRINT("KEY D TRIGGERED!");
					animation->setCurrentAnimationState("MoveRight");
				}

				bool b_isIdel = true;

				//Control Sprite KeyPress
				if (input->IsKeyPressed(GLFW_KEY_W)) {
					PRINT("KEY W PRESSED!");
					position->m_x += -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y += cos(glm::radians(0.0f)) * 5 * dt;
					b_isIdel = false;
				}
				if (input->IsKeyPressed(GLFW_KEY_S)) {
					PRINT("KEY S PRESSED!");
					position->m_x -= -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= cos(glm::radians(0.0f)) * 5 * dt;
					b_isIdel = false;
				}
				if (input->IsKeyPressed(GLFW_KEY_A)) {
					PRINT("KEY A PRESSED!");
					position->m_x -= cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= sin(glm::radians(0.0f)) * 5 * dt;
					b_isIdel = false;
				}
				if (input->IsKeyPressed(GLFW_KEY_D)) {
					PRINT("KEY D PRESSED!");
					position->m_x += cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y += sin(glm::radians(0.0f)) * 5 * dt;
					b_isIdel = false;
				}
				animation->GetCurrentAnimation()->SetAnimIdle(b_isIdel);
			}
		}
	};
}