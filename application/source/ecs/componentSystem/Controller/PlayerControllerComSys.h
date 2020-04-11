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
#include "tilemap/GameLevelMapManager.h"
#include "ui/GameWidgetManager.h"
#include "ecs/components/ActiveCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/CoolDownCom.h"
#include "ecs/components/ChildrenCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/CustomEvents.h"
#include "ecs/components/PlayerSkillComponent.h"
#include "skill-system/support-skills/MultipleProjectile.h"
#include "skill-system/active-skills/FireballAttack.h"
#include "skill-system/active-skills/IceballAttack.h"
#include "skill-manager/SkillManager.h"

#include <map>
#include <functional>

namespace gswy
{
	struct SkillBinding
	{

		SkillBinding()
		{
		}

		SkillBinding(int skillIndex, std::function<bool(const int&)> inputFunction)
			: m_skillIndex(skillIndex),
			m_inputFunction(inputFunction)
		{
		}

		int m_skillIndex;
		std::function<bool(const int&)> m_inputFunction;
	};

	class PlayerControllerComSys : public BaseComponentSystem<GameObjectType> {

	private:
		float m_speed = { 1.0f };
		int m_pathFindingLookAhead = { 4 };
		float m_noPathFindingThreshold_lower = { 0.15f };
		float m_noPathFindingThreshold_upper = { 3.f };
		float m_advancePathFindingThreshold = { 0.15f };
		double m_timeDisableMoveCommand = { 0 };
		bool m_bDisableMoveInput = { false };
		bool m_bDisableInput = { false };

		std::vector<glm::ivec2> m_pathResult;
		float m_cubicSplineStepSize = { .5f };
		std::vector<glm::vec2> m_pathResultCubicSplline;

		std::map<int, SkillBinding> m_keyAndSkillBiding;
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<PlayerSkillComponent>();
		}

		virtual void Init() override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerControllerComSys>(this, EventType::KEY_BIND_EVENT, &PlayerControllerComSys::OnKeyBindingEvent);
			queue->Subscribe<PlayerControllerComSys>(this, EventType::CAN_PLAYER_INPUT, &PlayerControllerComSys::OnCanPlayerInput);
		}

		void OnKeyBindingEvent(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto keyBindEvent = std::static_pointer_cast<KeyBindEvent>(e);
			InputManager* manager = InputManager::GetInstance();
			std::function<bool(const int&)> callback;
			if (keyBindEvent->m_keyEventType._Equal("TRIGGER"))
			{
				callback = std::bind(&InputManager::IsKeyTriggered, manager, std::placeholders::_1);
			}
			else
			{
				callback = std::bind(&InputManager::IsKeyPressed, manager, std::placeholders::_1);
			}
			SkillBinding skillBinding(keyBindEvent->m_skillNumber, callback);
			int key;
			switch (keyBindEvent->m_skillNumber)
			{
			case 1:
				key = KEY_Q;
				break;
			case 2:
				key = KEY_W;
				break;
			case 3:
				key = KEY_E;
				break;
			case 4:
				key = KEY_R;
				break;
			}

			m_keyAndSkillBiding[key] = skillBinding;
		}

		void OnCanPlayerInput(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<CanPlayerInputEvent>(e))
			{
				m_bDisableMoveInput = !event->m_bInput;
				m_bDisableInput = !event->m_bInput;
			}
		}

		virtual void Update(double dt) override {

			ProcessConstantInput();

			// Do not update player while game is paused
			if (!dt)
			{
				return;
			}

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto skillManager = SkillManager::GetInstance();

			// Handle mouse action
			if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
			{
				HandleMouseAction_LeftTriggered();
			}

			HandleMouseCondition_Move(dt);
			if (input->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				HandleMouseAction_LeftPressed();
			}
			HandlePlayerMovement(dt);

			// Handle UI input
			ProcessUIInput();

			// CHECKING FOR SKILL KEY BINDINGS
			for (const auto& keyBinding : m_keyAndSkillBiding)
			{
				if (keyBinding.second.m_inputFunction(keyBinding.first))
				{
					std::shared_ptr<ActiveSkill> skill = skillManager->GetActiveSkill(keyBinding.second.m_skillIndex);
					if (skill != nullptr)
					{
						auto e = MemoryManager::Make_shared<SkillUseEvent>(skill);
						queue->Publish(e);
					}
				}
			}

			if (input->IsKeyReleased(GLFW_KEY_UNKNOWN))
			{
				APP_CRITICAL("CRITICAL EVENT");
			}
		}

		void ProcessConstantInput()
		{
			auto credit_page = m_parentWorld->GetAllEntityWithType(GameObjectType::CREDITS);
			if (!credit_page.empty())
			{
				auto input = InputManager::GetInstance();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				if (input->IsKeyTriggered(KEY_ESCAPE) || input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(credit_page[0]);
					queue->Publish(e);
					// Set widget
					{
						WidgetManager* manager = WidgetManager::GetInstance();
						manager->GetPauseMenu().SetVisible(true);
					}
				}
			}
		}

		void ProcessUIInput()
		{
			// Handle UI inputs
			if (!m_bDisableInput)
			{
				auto input = InputManager::GetInstance();
				// Start level by pressing space
				if (input->IsKeyTriggered(KEY_SPACE))
				{
					PRINT("SPACE");
					if (!GameLevelMapManager::GetInstance()->IsWaveStarted())
					{
						GameLevelMapManager::GetInstance()->StartWave();
					}
				}

				// Handle UI toggling
				if (GameLevelMapManager::GetInstance()->IsInGame())
				{
					if (input->IsKeyTriggered(KEY_TAB))
					{
						bool v1 = WidgetManager::GetInstance()->GetShopMenu().GetVisible();
						WidgetManager::GetInstance()->GetShopMenu().SetVisible(!v1);
						m_bDisableMoveInput = !v1;
					}
					if (input->IsKeyTriggered(KEY_ESCAPE))
					{
						WidgetManager::GetInstance()->GetPauseMenu().SetVisible(!WidgetManager::GetInstance()->GetPauseMenu().GetVisible());
						m_parentWorld->SetPause(WidgetManager::GetInstance()->GetPauseMenu().GetVisible());
					}
				}
			}
		}

		bool HandleMouseCondition_Move(double dt)
		{
			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<BodyCom, GameObjectType> mouseBodyCom;
			m_parentWorld->Unpack(mouse, mouseBodyCom);

			bool couldMove = true;
			switch (mouseBodyCom->GetOtherEntity().m_type)
			{

			case GameObjectType::TOWER_BUILD: case GameObjectType::TOWER_FIRE: 
			case GameObjectType::TOWER_ICE: case GameObjectType::TOWER_LIGHTNING:
				m_timeDisableMoveCommand = 0.1;
				couldMove = false;
				break;
			default:
				break;
			}

			m_timeDisableMoveCommand -= dt;

			return couldMove;
		}

		void HandleMouseAction_LeftPressed()
		{
			// Click to move conditions
			if (m_timeDisableMoveCommand > 0 || m_bDisableMoveInput)
			{
				return;
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto tileMapObj = GameLevelMapManager::GetInstance()->GetCurrentMap();
			auto pathGrid = tileMapObj->GetTileGrid("PlayerBlock");
			auto Astar = tileMapObj->GetPathFinder("PlayerBlock");

			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			ComponentDecorator<TransformCom, GameObjectType> transform;
			ComponentDecorator<BodyCom, GameObjectType> body;
			ComponentDecorator<AnimationCom, GameObjectType> animation;
			m_parentWorld->Unpack(entity, transform);
			m_parentWorld->Unpack(entity, animation);
			m_parentWorld->Unpack(entity, body);
			auto playerPos = transform->GetPos();

			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<TransformCom, GameObjectType> mouseTransform;
			m_parentWorld->Unpack(mouse, mouseTransform);
			auto cursorPos = mouseTransform->GetPos();

			// Click to move
			auto dest = cursorPos;
			auto src = playerPos;
			auto delta = dest - src;
			// Stop when delta distance is small
			if (glm::length(delta) < m_noPathFindingThreshold_lower || glm::length(delta) > m_noPathFindingThreshold_upper)
			{
				body->SetVelocity(0,0);
				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Idle", false);
				queue->Publish(e);
				return;
			}
			auto _dest = tileMapObj->World2Grid(dest);
			auto _src = tileMapObj->World2Grid(src);

			m_pathResult.clear();
			m_pathResultCubicSplline.clear();
			if (Astar->Search(*pathGrid, _src, _dest))
			{
				m_pathResult = Astar->GetResult();
				int step = 1;
				for (int i = 0; i < m_pathResult.size()-step; i+=step)
				{
					m_pathResultCubicSplline.push_back(tileMapObj->Grid2World(m_pathResult[i]));
				}
				m_pathResultCubicSplline.push_back(tileMapObj->Grid2World(m_pathResult.back()));
				m_pathResultCubicSplline = GetCubicSpline(m_pathResultCubicSplline, m_cubicSplineStepSize);

				// Reverse path finding result from (start to dest) to (dest to start) for faster deletion in later step
				std::reverse(m_pathResultCubicSplline.begin(), m_pathResultCubicSplline.end());
				std::reverse(m_pathResult.begin(), m_pathResult.end());
			}
			else
			{
				PRINT(Str(entity) + " path not found");
			}
		}

		void HandleMouseAction_LeftTriggered()
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<BodyCom, GameObjectType> mouseBodyCom;
			m_parentWorld->Unpack(mouse, mouseBodyCom);

			switch (mouseBodyCom->GetOtherEntity().m_type)
			{
			case GameObjectType::TOWER_BUILD: case GameObjectType::TOWER_FIRE: case GameObjectType::TOWER_ICE: case GameObjectType::TOWER_LIGHTNING:
			{
				auto e = MemoryManager::Make_shared<ClickOnTowerEvent>(mouseBodyCom->GetOtherEntity());
				queue->Publish(e);
			}
				break;
			default:
				break;
			}
		}

		void HandlePlayerMovement(double dt)
		{
			// Do nothing if delta time has been set to 0 (game is paused)
			if (!dt)
			{
				return;
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto tileMapObj = GameLevelMapManager::GetInstance()->GetCurrentMap();
			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			ComponentDecorator<TransformCom, GameObjectType> transform;
			ComponentDecorator<AnimationCom, GameObjectType> animation;
			ComponentDecorator<BodyCom, GameObjectType> body;
			m_parentWorld->Unpack(entity, transform);
			m_parentWorld->Unpack(entity, body);
			m_parentWorld->Unpack(entity, animation);
			auto playerPos = body->GetPos();

			// Path finding end, set animation set to idle
			if (m_pathResultCubicSplline.empty() || m_pathResult.empty())
			{
				body->SetVelocity(vec2(0));
				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Idle", false);
				queue->Publish(e);
				return;
			}
			{
				//Update grid data
				auto pathsLeft_size = m_pathResult.size();
				auto nextPos = tileMapObj->Grid2World((pathsLeft_size > m_pathFindingLookAhead) ?
					m_pathResult[pathsLeft_size - 1 - m_pathFindingLookAhead] :
					m_pathResult.back());
				//1. Rotate
				auto delta = nextPos - playerPos;
				if (glm::length(delta) < m_advancePathFindingThreshold)
				{
					if (pathsLeft_size > m_pathFindingLookAhead)
					{
						m_pathResult.erase(m_pathResult.end() - 1 - m_pathFindingLookAhead, m_pathResult.end());
					}
					else
					{
						m_pathResult.pop_back();
					}
				}
				// 0. Set aimation
				auto e1 = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Move", false);
				queue->Publish(e1);
				// 1. Rotate
				transform->SetRotation(LookAt(delta));
				// 2. Move
				body->SetVelocity(glm::normalize(delta) * m_speed);
				// 3. Play sound
				auto e2 = MemoryManager::Make_shared<SoundEvent>("footsteps1", body->GetPos(), 1, 1.15);
				queue->Publish(e2);

			}
			{
				// Use cubic spline
				//auto pathsLeft_size = m_pathResultCubicSplline.size();
				//auto nextPos = tileMapObj->Grid2World((pathsLeft_size > m_pathFindingLookAhead) ?
				//	m_pathResultCubicSplline[pathsLeft_size - 1 - m_pathFindingLookAhead] :
				//	m_pathResultCubicSplline.back());
				////1. Rotate
				//auto delta = nextPos - playerPos;
				//if (glm::length(delta) < m_advancePathFindingThreshold)
				//{
				//	if (pathsLeft_size > m_pathFindingLookAhead)
				//	{
				//		m_pathResultCubicSplline.erase(m_pathResultCubicSplline.end() - 1 - m_pathFindingLookAhead, m_pathResultCubicSplline.end());
				//	}
				//	else
				//	{
				//		m_pathResultCubicSplline.pop_back();
				//	}
				//}
				//// 0. Set aimation
				//auto e1 = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Move", false);
				//queue->Publish(e1);
				//// 1. Rotate
				//transform->SetRotation(LookAt(delta));
				//// 2. Move
				//body->SetVelocity(glm::normalize(delta) * m_speed);
				//// 3. Play sound
				//auto e2 = MemoryManager::Make_shared<SoundEvent>("footstep02", body->GetPos(), 1, 0.65);
				//queue->Publish(e2);
			}
		}
	};
}