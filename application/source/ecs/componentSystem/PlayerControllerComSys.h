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

				if (entity.m_type != GameObjectType::PLAYER)
					continue;

				{
					auto input = InputManager::GetInstance();
					ComponentDecorator<TransformCom, GameObjectType> position;
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, position);
					m_parentWorld->Unpack(entity, animation);

					// 1. Making player facing the cursor
					auto cursor = input->GetCursorPosition();
					auto center = input->GetCursorMaxPosition() * 0.5f;
					auto delta = cursor - center;
					delta.y = -delta.y;
					position->SetRotation(LookAt(delta));

					if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
					{
						PRINT(MOUSE_BUTTON_LEFT);
					}

					// 2. Movement with keys
					bool isIdle = true;
					vec2 velocity(0);
					float speed = 1.0f;
					if (input->IsKeyPressed(KEY_W) && input->IsAllKeyNotPressed<int>(KEY_S)) {
						PRINT("KEY W PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(sinf(glm::radians(0.0f)), cosf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_S) && input->IsAllKeyNotPressed<int>(KEY_W)) {
						PRINT("KEY S PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(sinf(glm::radians(0.0f)), -cosf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_A) && input->IsAllKeyNotPressed<int>(KEY_D)) {
						PRINT("KEY A PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(-cosf(glm::radians(0.0f)), sinf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_D) && input->IsAllKeyNotPressed<int>(KEY_A)) {
						PRINT("KEY D PRESSED!");
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(cosf(glm::radians(0.0f)), sinf(glm::radians(0.0f)));	
					}
					
					if (!isIdle)
					{
						if (!AudioManager::GetInstance()->IsPlaying("footstep02"))
							AudioManager::GetInstance()->PlaySound("footstep02", AudioVector3{ 0, 0, 0 }, 1, 1.0);
						position->AddXY(glm::normalize(velocity) * speed * (float)dt);
					}
					animation->GetCurrentAnimation()->SetAnimIdle(isIdle);
					return;
				}
			}
		}
	};
}