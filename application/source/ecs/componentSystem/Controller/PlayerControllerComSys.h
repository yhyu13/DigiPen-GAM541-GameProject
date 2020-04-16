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
#include "ecs/components/HitPointCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/CustomEvents.h"
#include "ecs/components/PlayerSkillComponent.h"
#include "skill-system/support-skills/MultipleProjectile.h"
#include "skill-system/active-skills/FireballAttack.h"
#include "skill-system/active-skills/IceballAttack.h"
#include "skill-manager/SkillManager.h"
#include "HowToPlayState.h"

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
		bool m_cheatEnabled = { false };
		bool m_autoPlayEnabled = { false };
		double m_autoPlayPathFindingCoolDown = { .5 };
		double m_autoPlayPathFindingCoolDown_ = m_autoPlayPathFindingCoolDown;
		double m_autoPlayApplySkillCoolDown = { .5 };
		double m_autoPlayApplySkillCoolDown_ = m_autoPlayApplySkillCoolDown;

		float m_speed = { 1.0f };
		int m_pathFindingLookAhead = { 4 };
		float m_noPathFindingThreshold_lower = { 0.15f };
		float m_noPathFindingThreshold_upper = { 3.f };
		float m_advancePathFindingThreshold = { 0.15f };
		double m_timeDisableMoveCommand = { 0 };
		bool m_bDisableMoveInput = { false };
		bool m_bDisableInput = { false };
		HowToPlayState m_howToPlayState = { HowToPlayState::HOW_TO_PLAY_1 };

		std::vector<glm::ivec2> m_pathResult;
		bool m_cubicSplineEnabled = { false };	// Cubic spline refinement of path finding should be disabled due to unstable result
		float m_cubicSplineStepSize = { .5f };
		std::vector<glm::vec2> m_pathResultCubicSplline;

		std::map<int, SkillBinding> m_keyAndSkillBiding;
	public:
		PlayerControllerComSys() {
			m_systemSignature.AddComponent<PlayerSkillComponent>();
		}

		virtual void Init() override
		{
			Json::Value engineConfiguration;
			std::ifstream file("./asset/engine-configuration/engine-config.json", std::ifstream::binary);
			file >> engineConfiguration;
			file.close();
			m_cheatEnabled = engineConfiguration["enable-cheat"].asBool();

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerControllerComSys>(this, EventType::KEY_BIND_EVENT, &PlayerControllerComSys::OnKeyBindingEvent);
			queue->Subscribe<PlayerControllerComSys>(this, EventType::CAN_PLAYER_INPUT, &PlayerControllerComSys::OnCanPlayerInput);
			queue->Subscribe<PlayerControllerComSys>(this, EventType::SOUND_AT_PLAYER, &PlayerControllerComSys::OnPlaySoundAtPlayerLocation);
			queue->Subscribe<PlayerControllerComSys>(this, EventType::LOAD_HOW_TO_PLAY, &PlayerControllerComSys::OnLoadHowToPlay);
		}

		void OnLoadHowToPlay(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<LoadHowToPlayEvent>(e))
			{
				m_howToPlayState = HowToPlayState::HOW_TO_PLAY_1;
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

		void OnCanPlayerInput(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<CanPlayerInputEvent>(e))
			{
				m_bDisableMoveInput = !event->m_bInput;
				m_bDisableInput = !event->m_bInput;
			}
		}

		void OnPlaySoundAtPlayerLocation(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<PlaySoundAtPlayerLocationEvent>(e))
			{
				auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
				ComponentDecorator<BodyCom, GameObjectType> body;
				m_parentWorld->Unpack(entity, body);
				auto playerPos = body->GetPos();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto e1 = MemoryManager::Make_shared<SoundEvent>(event->soundName, playerPos, event->m_vol, event->m_freq);
				queue->Publish(e1);
			}
		}

		virtual void Update(double dt) override {

			{
				// Process constant input regardless of whether the game is paused or not
				ProcessConstantInput();
			}

			// Do not update player while game is paused
			if (!dt)
			{
				return;
			}

			auto input = InputManager::GetInstance();

			{
				// Handle mouse action				
				HandleMouseAction_LeftTriggered(dt);
				// Handle move conditions
				HandleMouseCondition_Move(dt);
				HandleMouseAction_LeftPressed(dt);
				// Update movement
				HandlePlayerMovement(dt);
				// Update skill
				HandlePlayerSkill(dt);
			}
			
			{
				// Handle Cheat
				ProcessCheatInput();
				// Handle UI input
				ProcessUIInput();
			}
		}

		void ProcessConstantInput()
		{
			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			// Process showing and hiding of the game credit, how to play page
			auto credit = m_parentWorld->GetAllEntityWithType(GameObjectType::CREDITS);
			auto how_to_play = m_parentWorld->GetAllEntityWithType(GameObjectType::HOW_TO_PLAY);

			if (!credit.empty())
			{
				auto entity = credit[0];
				if (input->IsKeyTriggered(KEY_ESCAPE)
					|| input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT)
					|| input->IsMouseButtonTriggered(MOUSE_BUTTON_RIGHT)
					|| input->IsKeyTriggered(KEY_SPACE))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(entity);
					queue->Publish(e);
					// Set main menu back to visible
					{
						WidgetManager::GetInstance()->GetPauseMenu().SetVisible(true);
					}
				}
				return;
			}
			else if (!how_to_play.empty())
			{
				auto entity = how_to_play[0];
				if (input->IsKeyTriggered(KEY_ESCAPE))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(entity);
					queue->Publish(e);
					// Set main menu back to visible
					{
						WidgetManager::GetInstance()->GetPauseMenu().SetVisible(true);
					}
				}
				else
				{
					switch (m_howToPlayState)
					{
					case HowToPlayState::HOW_TO_PLAY_1:
					{
						if (input->IsKeyTriggered(KEY_DOWN))
						{
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_2");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_2;
						}
					}
					break;

					case HowToPlayState::HOW_TO_PLAY_2:
					{
						if (input->IsKeyTriggered(KEY_UP))
						{
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_1");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_1;
						}
						else if (input->IsKeyTriggered(KEY_DOWN))
						{
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_3");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_3;
						}
					}
					break;

					case HowToPlayState::HOW_TO_PLAY_3:
					{
						if (input->IsKeyTriggered(KEY_UP))
						{
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_2");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_2;
						}
					}
					break;
					default:
						break;
					}
				}
				return;
			}
			// Toggle the pause menu
			else if (input->IsKeyTriggered(KEY_ESCAPE))
			{
				// Turn off the option menu anyway
				auto& option = WidgetManager::GetInstance()->GetOptionMenu();
				option.SetVisible(false);

				// Toggle the pause menu and the BGM
				auto& pause = WidgetManager::GetInstance()->GetPauseMenu();
				m_parentWorld->SetPause(!m_parentWorld->IsPaused());
				pause.SetVisible(m_parentWorld->IsPaused());
				//Pause track
				AudioManager::GetInstance()->SetSoundPause("Track_1", m_parentWorld->IsPaused());
				return;
			}
		}

		void ProcessCheatInput()
		{
			if (!m_cheatEnabled)
			{
				return;
			}

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			// Toggle auto play
			if (input->IsKeyTriggered(KEY_F1))
			{
				m_autoPlayEnabled = !m_autoPlayEnabled;
				m_autoPlayPathFindingCoolDown_ = m_autoPlayPathFindingCoolDown;
			}

			// Add coins 
			if (input->IsKeyTriggered(KEY_F2))
			{
				GameLevelMapManager::GetInstance()->AddCoins(1000);
			}

			// Tooggle lock player HP
			if (input->IsKeyTriggered(KEY_F3))
			{
				auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
				auto HP = GetComponent<HitPointCom>(entity);
				HP->SetHitPointLock(!HP->IsHitPointLock());
			}

			// Tooggle lock base HP
			if (input->IsKeyTriggered(KEY_F4))
			{
				auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
				auto HP = GetComponent<HitPointCom>(entity);
				HP->SetHitPointLock(!HP->IsHitPointLock());
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
				// Create a tiny time period where the click to move is diabled
				// Otherwise, the player will move the tower where the click is happend
				m_timeDisableMoveCommand = 0.1;
				couldMove = false;
				break;
			default:
				break;
			}

			m_timeDisableMoveCommand -= dt;
			return couldMove;
		}

		void HandleMouseAction_LeftPressed(double dt)
		{
			// function static variable that stores the target location 
			// if autoplay is enabled
			static vec2 autoPlayTargetPos = vec2(0);

			// Click to move conditions
			if (m_timeDisableMoveCommand > 0 || m_bDisableMoveInput || m_bDisableInput)
			{
				return;
			}
			auto input = InputManager::GetInstance();
			if (!m_autoPlayEnabled && !input->IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
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
			autoPlayTargetPos = playerPos;
			auto cursorPos = autoPlayTargetPos;

			if (m_autoPlayEnabled)
			{
				// Exectue auto path finding when cool down is ready
				if (m_autoPlayPathFindingCoolDown_ == m_autoPlayPathFindingCoolDown)
				{
					std::vector<Entity<GameObjectType>> allEnemies;
					int trails = 5;
					// Get a specific type of enemy as targets
					while (allEnemies.empty() && (--trails > 0))
					{
						allEnemies = m_parentWorld->GetAllEntityWithType(g_enemyTypes[RAND_I(0, g_enemyTypes.size())]);
					}
					if (!allEnemies.empty())
					{
						bool find_nearest = false;
						vec2 closest_enmey_delta(5);
						for (auto& entity : allEnemies) {
							ComponentDecorator<TransformCom, GameObjectType> transform;
							m_parentWorld->Unpack(entity, transform);
							auto enmey_pos = transform->GetPos();
							auto delta = enmey_pos - playerPos;

							// Find the cloest enmey position
							if (glm::length(delta) < glm::length(closest_enmey_delta))
							{
								closest_enmey_delta = delta;
								cursorPos = enmey_pos;
								autoPlayTargetPos = enmey_pos;
								find_nearest = true;
							}
						}
					}
				}
				// Update cool down
				if ((m_autoPlayPathFindingCoolDown_ -= dt) < 0)
				{
					m_autoPlayPathFindingCoolDown_ = m_autoPlayPathFindingCoolDown;
				}
			}
			else
			{
				auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
				ComponentDecorator<TransformCom, GameObjectType> mouseTransform;
				m_parentWorld->Unpack(mouse, mouseTransform);
				cursorPos = mouseTransform->GetPos();
			}

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
				for (int i = 0; i < m_pathResult.size() - step; i += step)
				{
					m_pathResultCubicSplline.push_back(tileMapObj->Grid2World(m_pathResult[i]));
				}
				m_pathResultCubicSplline.push_back(tileMapObj->Grid2World(m_pathResult.back()));


				if (m_cubicSplineEnabled)
				{
					// Calculate cubic spline curve from given points (could be costy)
					m_pathResultCubicSplline = GetCubicSpline(m_pathResultCubicSplline, m_cubicSplineStepSize);
				}

				// Reverse path finding result from (start to dest) to (dest to start) for faster deletion in later step
				std::reverse(m_pathResultCubicSplline.begin(), m_pathResultCubicSplline.end());
				std::reverse(m_pathResult.begin(), m_pathResult.end());
			}
			else
			{
				DEBUG_PRINT(Str(entity) + " path not found");
			}
		}

		void HandleMouseAction_LeftTriggered(double dt)
		{
			auto input = InputManager::GetInstance();
			if (!input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || m_bDisableInput)
			{
				return;
			}

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
			if (m_pathResultCubicSplline.empty() || m_pathResult.empty() || m_bDisableMoveInput || m_bDisableInput)
			{
				body->SetVelocity(vec2(0));
				auto e = MemoryManager::Make_shared<PlayerSetPendingAnimationEvent>(entity, "Idle", false);
				queue->Publish(e);
				return;
			}

			
			if (m_cubicSplineEnabled)
			{
				// Cubic spline refinement of path finding is disabled due to unstable result
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
			else
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
				auto audio = AudioManager::GetInstance();
				if (!audio->IsPlaying("footsteps1"))
				{
					auto e3 = MemoryManager::Make_shared<PlaySoundAtPlayerLocationEvent>("footsteps1", 1, 1.15);
					queue->Publish(e3);
				}
			}
		}

		void HandlePlayerSkill(double dt)
		{
			// Do nothing if delta time has been set to 0 (game is paused)
			if (!dt || m_bDisableInput)
			{
				return;
			}

			// Checking for skill key binding
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto skillManager = SkillManager::GetInstance();
			if (m_autoPlayEnabled)
			{
				// Exectue auto path finding when cool down is ready
				if (m_autoPlayApplySkillCoolDown_ == m_autoPlayApplySkillCoolDown)
				{
					std::vector<std::shared_ptr<ActiveSkill>> skills;
					for (const auto& keyBinding : m_keyAndSkillBiding)
					{
						if (auto skill = skillManager->GetActiveSkill(keyBinding.second.m_skillIndex))
						{
							skills.push_back(skill);
						}
					}
					// Apply a random active skill
					if (!skills.empty())
					{
						auto e = MemoryManager::Make_shared<SkillUseEvent>(skills[RAND_I(0, skills.size())]);
						queue->Publish(e);
					}
				}
				// Update cool down
				if ((m_autoPlayApplySkillCoolDown_ -= dt) < 0)
				{
					m_autoPlayApplySkillCoolDown_ = m_autoPlayApplySkillCoolDown;
				}
			}
			else
			{
				for (const auto& keyBinding : m_keyAndSkillBiding)
				{
					if (keyBinding.second.m_inputFunction(keyBinding.first))
					{
						if (auto skill = skillManager->GetActiveSkill(keyBinding.second.m_skillIndex))
						{
							auto e = MemoryManager::Make_shared<SkillUseEvent>(skill);
							queue->Publish(e);
						}
					}
				}
			}
		}
	};
}