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
			:
			m_skillIndex(0)
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
		std::vector<glm::ivec2> m_pathResult;
		float m_speed = { 1.0f };
		int m_pathFindingLookAhead = { 2 };
		float m_noPathFindingThreshold = { 0.15f };
		float m_advancePathFindingThreshold = { 0.1f };
		float m_maxAngleRotation = { 0.8726646f };

		double m_timeDisableMoveCommand = { 0 };
		bool m_bDisableMoveCommand = { false };
		bool m_bDisableInput = { false };
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<PlayerSkillComponent>();
		}

		virtual void Init() override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerControllerComSys>(this, EventType::KEY_BIND_EVENT, &PlayerControllerComSys::OnKeyBindingEvent);
			queue->Subscribe<PlayerControllerComSys>(this, EventType::CAN_PLAYER_INPUT, &PlayerControllerComSys::OnCanPlayerInputEvent);
		}

		void OnCanPlayerInputEvent(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<CanPlayerInputEvent>(e))
			{
				m_bDisableInput = !event->m_bInput;
			}
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

		virtual void Update(double dt) override {

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto skillManager = SkillManager::GetInstance();

			if (m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER).empty())
			{
				return;
			}
			if (m_bDisableInput)
			{
				return;
			}

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

			// Start level by pressing space
			if (input->IsKeyTriggered(KEY_SPACE))
			{
				PRINT("SPACE");
				if (!GameLevelMapManager::GetInstance()->IsWaveStarted() && !GameLevelMapManager::GetInstance()->IsLoading())
				{
					GameLevelMapManager::GetInstance()->StartWave();
				}
			}

			// Handle UI toggling
			if (GameLevelMapManager::GetInstance()->IsInGame())
			{
				if (input->IsKeyTriggered(KEY_I))
				{
					auto& hud = WidgetManager::GetInstance()->GetInventoryMenu();
					hud.SetVisible(!hud.GetVisible());
				}
				if (input->IsKeyTriggered(KEY_P))
				{
					auto& hud = WidgetManager::GetInstance()->GetShopMenu();
					hud.SetVisible(!hud.GetVisible());
				}

				// Disable movement while either inventory or shop menu is active
				m_bDisableMoveCommand = WidgetManager::GetInstance()->GetShopMenu().GetVisible() || WidgetManager::GetInstance()->GetInventoryMenu().GetVisible();

				if (input->IsKeyTriggered(KEY_ESCAPE))
				{
					auto& hud = WidgetManager::GetInstance()->GetPauseMenu();
					hud.SetVisible(!hud.GetVisible());
					m_parentWorld->SetPause(hud.GetVisible());
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
			if (m_timeDisableMoveCommand > 0 || m_bDisableMoveCommand)
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
			if (glm::length(delta) < m_noPathFindingThreshold)
			{
				//transform->SetVelocity(vec2(0));
				body->SetVelocity(0,0);
				/*animation->SetCurrentAnimationState("Idle");*/\
				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Idle", false);
				queue->Publish(e);
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

		// TODO : Optimize the path result by Cubic spline
		void HandlePlayerMovement(double dt)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			ComponentDecorator<TransformCom, GameObjectType> transform;
			ComponentDecorator<AnimationCom, GameObjectType> animation;
			ComponentDecorator<BodyCom, GameObjectType> body;
			m_parentWorld->Unpack(entity, transform);
			m_parentWorld->Unpack(entity, body);
			m_parentWorld->Unpack(entity, animation);

			if (m_pathResult.empty())
			{
				body->SetVelocity(vec2(0));
				//animation->SetCurrentAnimationState("Idle");
				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Idle", false);
				queue->Publish(e);
				return;
			}

			auto audio = AudioManager::GetInstance();
			auto tileMapObj = GameLevelMapManager::GetInstance()->GetCurrentMap();
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
				//PRINT(m_pathResult.size());
			}
			auto angle = LookAt(delta);
			// TODO: need fine tune for smooth player turning
			//if (angle > m_maxAngleRotation)
			//{
			//	angle = m_maxAngleRotation;
			//}
			//else if (angle < -m_maxAngleRotation)
			//{
			//	angle = -m_maxAngleRotation;
			//}
			if (dt)
			{
				transform->SetRotation(angle);
			}
			// 2. Move
			body->SetVelocity(glm::normalize(delta) * m_speed);
			//animation->SetCurrentAnimationState("Move");
			auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Move", false);
			queue->Publish(e);

			// 3. Play sound
			if (dt)
			{
				auto e = MemoryManager::Make_shared<SoundEvent>("footsteps", vec3(0), 1, 1.15);
				queue->Publish(e);
			}
		}

		private:

			std::map<int, SkillBinding> m_keyAndSkillBiding;
	};
}