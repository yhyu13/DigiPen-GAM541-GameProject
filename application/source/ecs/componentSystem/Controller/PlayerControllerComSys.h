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
#include "ecs/CustomEvents.h"

namespace gswy
{
	class PlayerControllerComSys : public BaseComponentSystem<GameObjectType> {
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<AnimationCom>();
		}

		virtual void Update(double dt) override {

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto audio = AudioManager::GetInstance();

			for (auto& entity : m_registeredEntities) {

				if (entity.m_type != GameObjectType::PLAYER)
					continue;

				{
					ComponentDecorator<TransformCom, GameObjectType> transform;
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, transform);
					m_parentWorld->Unpack(entity, animation);

					// 1. Making player facing the cursor
					auto cursor = input->GetCursorPosition();
					auto center = input->GetCursorMaxPosition() * 0.5f;
					auto delta = cursor - center;
					delta.y = -delta.y;
					transform->SetRotation(LookAt(delta));

					// 2. Fire
					if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
					{
						FireWeaponEvent e(entity);
						queue->Publish(&e);
					}

					// 3. (Demo) Spawn enemies by triggering buttons
					if (input->IsKeyTriggered(KEY_SPACE))
					{
						SpawnEvent e(GameObjectType::ENEMY, vec3(RAND_F(-1,1), RAND_F(-1, 1), 0));
						queue->Publish(&e);
					}

					// 4. Movement with keys
					bool isIdle = true;
					vec2 velocity(0);
					float speed = 1.0f;
					transform->SetVelocity(velocity * speed);
					if (input->IsKeyPressed(KEY_W) && input->IsAllKeyNotPressed<int>(KEY_S)) {
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(sinf(glm::radians(0.0f)), cosf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_S) && input->IsAllKeyNotPressed<int>(KEY_W)) {
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(sinf(glm::radians(0.0f)), -cosf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_A) && input->IsAllKeyNotPressed<int>(KEY_D)) {
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(-cosf(glm::radians(0.0f)), sinf(glm::radians(0.0f)));
					}
					if (input->IsKeyPressed(KEY_D) && input->IsAllKeyNotPressed<int>(KEY_A)) {
						isIdle = false;
						animation->SetCurrentAnimationState("Move");
						velocity += vec2(cosf(glm::radians(0.0f)), sinf(glm::radians(0.0f)));	
					}
					if (!isIdle)
					{
						// Play foot step sound
						SoundEvent e("footstep02");
						queue->Publish(&e);

						// Set moving velocity
						transform->SetVelocity(glm::normalize(velocity) * speed);
					}
					animation->GetCurrentAnimation()->SetAnimIdle(isIdle);
					return;
				}
			}
		}
	};
}