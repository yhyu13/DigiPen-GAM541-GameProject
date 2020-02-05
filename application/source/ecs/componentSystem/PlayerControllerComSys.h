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
#include "ecs/components/SpriteCom.h"

namespace gswy
{
	class PlayerControllerComSys : public BaseComponentSystem {
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void Update(double dt) override {
			for (auto& entity : m_registeredEntities) {

				auto input = Input::GetInstance();
				ComponentDecorator<TransformCom> position;
				ComponentDecorator<SpriteCom> sprite;
				m_parentWorld->Unpack(entity, position, sprite);
				auto m_ControlSprite = sprite->Get();
				

				//Control Sprite Trigger
				if (input->IsKeyTriggered(GLFW_KEY_W)) {
					PRINT("KEY W TRIGGERED!");
					m_ControlSprite->SetAnimSequence(16, 8);
					m_ControlSprite->SetAnimLooping(true);
				}
				else if (input->IsKeyTriggered(GLFW_KEY_S)) {
					PRINT("KEY S TRIGGERED!");
					m_ControlSprite->SetAnimSequence(24, 8);
					m_ControlSprite->SetAnimLooping(true);
				}
				else if (input->IsKeyTriggered(GLFW_KEY_A)) {
					PRINT("KEY A TRIGGERED!");
					m_ControlSprite->SetAnimSequence(8, 8);
					m_ControlSprite->SetAnimLooping(true);
				}
				else if (input->IsKeyTriggered(GLFW_KEY_D)) {
					PRINT("KEY D TRIGGERED!");
					m_ControlSprite->SetAnimSequence(0, 8);
					m_ControlSprite->SetAnimLooping(true);
				}

				// Reset animation to idel state
				m_ControlSprite->SetAnimIdle(true);

				//Control Sprite KeyPress
				if (input->IsKeyPressed(GLFW_KEY_W)) {
					PRINT("KEY W PRESSED!");
					position->m_x += -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y += cos(glm::radians(0.0f)) * 5 * dt;
					m_ControlSprite->SetAnimIdle(false);
				}
				else if (input->IsKeyPressed(GLFW_KEY_S)) {
					PRINT("KEY S PRESSED!");
					position->m_x -= -sin(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= cos(glm::radians(0.0f)) * 5 * dt;
					m_ControlSprite->SetAnimIdle(false);
				}

				if (input->IsKeyPressed(GLFW_KEY_A)) {
					PRINT("KEY A PRESSED!");
					position->m_x -= cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y -= sin(glm::radians(0.0f)) * 5 * dt;
					m_ControlSprite->SetAnimIdle(false);
				}
				else if (input->IsKeyPressed(GLFW_KEY_D)) {
					PRINT("KEY D PRESSED!");
					position->m_x += cos(glm::radians(0.0f)) * 5 * dt;
					position->m_y += sin(glm::radians(0.0f)) * 5 * dt;
					m_ControlSprite->SetAnimIdle(false);
				}
			}
		}
	};
}