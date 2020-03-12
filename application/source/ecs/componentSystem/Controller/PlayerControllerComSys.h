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
#include "engine/ai/PathFinding.h"
#include "tilemap/GameTileMapManager.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class PlayerControllerComSys : public BaseComponentSystem<GameObjectType> {

	private:
		std::vector<glm::ivec2> m_pathResult;
		float m_speed = { 1.0f };
		int m_pathFindingLookAhead = { 2 };
		float m_noPathFindingThreshold = { 0.15f };
		float m_advancePathFindingThreshold = { 0.1f };
		float m_maxAngleRotation = { 0.8726646f};
	public:
		PlayerControllerComSys() {
		}

		virtual void Update(double dt) override {

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			//// 1. Making player facing the cursor
			//auto cursor = input->GetCursorPosition();
			//auto center = input->GetCursorMaxPosition() * 0.5f;
			//auto delta = cursor - center;
			//delta.y = -delta.y;
			//transform->SetRotation(LookAt(delta));

			// Handle mouse action
			if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
			{
				HandleMouseAction();
			}

			HandlePlayerMovement();

			//// TODO remove spawn enemies by triggering buttons
			//if (input->IsKeyTriggered(KEY_SPACE))
			//{
			//	auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::ENEMY, vec3(RAND_F(-1,1), RAND_F(-1, 1), 0));
			//	queue->Publish(e);

			//	static bool flash = true;
			//	ComponentDecorator<SpriteCom, GameObjectType> sprite;
			//	m_parentWorld->Unpack(entity, sprite);
			//	auto s = sprite->Get();
			//	(flash)? s->SetSpriteShader("White"): s->SetSpriteShader("Default");
			//	flash = !flash;
			//}

			// (Demo) Spawn tower by pressing keys
			if (input->IsKeyTriggered(KEY_P))
			{
				PRINT("P");
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0], transform);
				auto cursor_pos = transform->GetPos();
				auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::TOWER_BUILD, vec3(cursor_pos, 0));
				queue->Publish(e);
			}
		}

		void HandleMouseAction()
		{
			auto tileMapObj = GameTileMapManager::GetInstance()->GetCurrentMap();
			auto pathGrid = tileMapObj->GetTileGrid("Path");
			auto Astar = tileMapObj->GetPathFinder("Path");

			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			ComponentDecorator<TransformCom, GameObjectType> transform;
			ComponentDecorator<AnimationCom, GameObjectType> animation;
			m_parentWorld->Unpack(entity, transform);
			m_parentWorld->Unpack(entity, animation);
			auto playerPos = transform->GetPos();

			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<TransformCom, GameObjectType> mouseTransform;
			ComponentDecorator<BodyCom, GameObjectType> mouseBodyCom;
			m_parentWorld->Unpack(mouse, mouseTransform);
			m_parentWorld->Unpack(mouse, mouseBodyCom);
			auto cursorPos = mouseTransform->GetPos();

			bool isIdle = true;

			switch (mouseBodyCom->GetOtherEntity().m_type)
			{
			case GameObjectType::TOWER_BUILD:
			{

			}
			break;
			case GameObjectType::TOWER_FIRE:
			{

			}
			break;
			case GameObjectType::TOWER_ICE:
			{

			}
			break;
			case GameObjectType::TOWER_LIGHTNING:
			{

			}
			return;
			default:
				isIdle = false;
				break;
			}

			// Click to move
			if (isIdle)
			{
				return;
			}
			auto dest = cursorPos;
			auto src = playerPos;
			auto delta = dest - src;
			// Stop when delta distance is small
			if (glm::length(delta) < m_noPathFindingThreshold)
			{
				transform->SetVelocity(vec2(0));
				animation->SetCurrentAnimationState("Idle");
				return;
			}

			auto _dest = tileMapObj->World2Grid(dest);
			auto _src = tileMapObj->World2Grid(src);

			if (Astar->Search(*pathGrid, _src, _dest))
			{
				m_pathResult = Astar->GetResult();
				std::reverse(m_pathResult.begin(), m_pathResult.end());
			}
			else
			{
				PRINT(Str(entity) + " not found");
				m_pathResult.clear();
			}
		}

		void HandlePlayerMovement()
		{
			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			ComponentDecorator<TransformCom, GameObjectType> transform;
			ComponentDecorator<AnimationCom, GameObjectType> animation;
			m_parentWorld->Unpack(entity, transform);
			m_parentWorld->Unpack(entity, animation);

			if (m_pathResult.empty())
			{
				transform->SetVelocity(vec2(0));
				animation->SetCurrentAnimationState("Idle");
				return;
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto audio = AudioManager::GetInstance();
			auto tileMapObj = GameTileMapManager::GetInstance()->GetCurrentMap();
			auto playerPos = transform->GetPos();

			// Update click to move

			// 1. Rotate
			auto pathsLeft = m_pathResult.size();
			auto nextPos = tileMapObj->Grid2World((pathsLeft > m_pathFindingLookAhead) ? m_pathResult[pathsLeft-1-m_pathFindingLookAhead]: m_pathResult.back());
			auto delta = nextPos - playerPos;
			if (glm::length(delta) < m_advancePathFindingThreshold)
			{
				if (pathsLeft > m_pathFindingLookAhead)
				{
					m_pathResult.erase(m_pathResult.end() - 1 - m_pathFindingLookAhead, m_pathResult.end());
				}
				else
				{
					m_pathResult.pop_back();
				}
				PRINT(m_pathResult.size());
			}
			auto angle = LookAt(delta);
			// TODO: need fine tune
			//if (angle > m_maxAngleRotation)
			//{
			//	angle = m_maxAngleRotation;
			//}
			//else if (angle < -m_maxAngleRotation)
			//{
			//	angle = -m_maxAngleRotation;
			//}
			transform->SetRotation(angle);
			// 2. Move
			transform->SetVelocity(glm::normalize(delta) * m_speed);
			animation->SetCurrentAnimationState("Move");

			// 3. Play sound
			auto e = MemoryManager::Make_shared<SoundEvent>("footstep02");
			queue->Publish(e);
		}
	};
}