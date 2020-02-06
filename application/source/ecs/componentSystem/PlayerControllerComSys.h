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

namespace gswy
{
	class PlayerControllerComSys : public BaseComponentSystem {
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<AnimationCom>();
		}

		virtual void Update(double dt) override {
			for (auto& entity : m_registeredEntities) {

				auto input = Input::GetInstance();
				ComponentDecorator<TransformCom> position;
				ComponentDecorator<AnimationCom> animation;
				m_parentWorld->Unpack(entity, position, animation);
				
				//Control Sprite Trigger
				if (input->IsKeyTriggered(GLFW_KEY_W)) {
					PRINT("KEY W TRIGGERED!");
					animation->setCurrentAnimationState("Move1");
					/*m_ControlSprite->SetAnimSequence(16, 8);
					m_ControlSprite->SetAnimLooping(true);*/
				}
				else if (input->IsKeyTriggered(GLFW_KEY_S)) {
					PRINT("KEY S TRIGGERED!");
					animation->setCurrentAnimationState("Move2");
					/*
					m_ControlSprite->SetAnimSequence(24, 8);
					m_ControlSprite->SetAnimLooping(true);*/
				}
				else if (input->IsKeyTriggered(GLFW_KEY_A)) {
					PRINT("KEY A TRIGGERED!");
					animation->setCurrentAnimationState("Move3");
					/*m_ControlSprite->SetAnimSequence(8, 8);
					m_ControlSprite->SetAnimLooping(true);*/
				}
				else if (input->IsKeyTriggered(GLFW_KEY_D)) {
					PRINT("KEY D TRIGGERED!");
					animation->setCurrentAnimationState("Move3");
				/*	m_ControlSprite->SetAnimSequence(0, 8);
					m_ControlSprite->SetAnimLooping(true);*/
				}

				//Control Sprite KeyPress
				if (input->IsKeyPressed(GLFW_KEY_W)) {
					PRINT("KEY W PRESSED!");
					position->m_x += -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y += cos(glm::radians(0.0f)) * 5 * dt;
					
				}
				else if (input->IsKeyPressed(GLFW_KEY_S)) {
					PRINT("KEY S PRESSED!");
					position->m_x -= -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= cos(glm::radians(0.0f)) * 5 * dt;
					
				}

				if (input->IsKeyPressed(GLFW_KEY_A)) {
					PRINT("KEY A PRESSED!");
					position->m_x -= cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= sin(glm::radians(0.0f)) * 5 * dt;
					
				}
				else if (input->IsKeyPressed(GLFW_KEY_D)) {
					PRINT("KEY D PRESSED!");
					position->m_x += cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y += sin(glm::radians(0.0f)) * 5 * dt;
					
				}
			}
		}
	};
}