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
			m_systemSignature.AddComponent<PlayerSkillComponent>();
		}

		virtual void Update(double dt) override {

			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			bool b_shoudMove = HandleMouseCondition_Move();

			// Handle mouse action
			if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
			{
				HandleMouseAction_LeftTriggered();
			}

			if (b_shoudMove && input->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				HandleMouseAction_LeftPressed();
			}

			HandlePlayerMovement(dt);

			//// TODO remove spawn enemies by triggering buttons
			//if (input->IsKeyTriggered(KEY_T))
			//{
			//	auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::ENEMY_1, vec3(RAND_F(-1,1), RAND_F(-1, 1), 0));
			//	queue->Publish(e);
			//
			//	static bool flash = true;
			//	//auto enemy = m_parentWorld->GetAllEntityWithType(GameObjectType::ENEMY)[0];
			//	//auto enemy = m_registeredEntities.at(1);
			//	ComponentDecorator<SpriteCom, GameObjectType> sprite;
			//	m_parentWorld->Unpack(enemy, sprite);
			//	auto s = sprite->Get();
			//	(flash)? s->SetSpriteShader("White"): s->SetSpriteShader("Default");
			//	flash = !flash;
			//}

			SkillManager* skillManager = SkillManager::GetInstance();

			if (input->IsKeyTriggered(KEY_Q)) // FIRE-BALL
			{
				std::shared_ptr<ActiveSkill> skill = skillManager->GetActiveSkill(1);
				if (skill != nullptr)
				{
					auto e = MemoryManager::Make_shared<SkillUseEvent>(skill);
					queue->Publish(e);
				}
			}

			if (input->IsKeyTriggered(KEY_W)) // ICE-BALL
			{
				std::shared_ptr<ActiveSkill> skill = skillManager->GetActiveSkill(2);
				if (skill != nullptr)
				{
					auto e = MemoryManager::Make_shared<SkillUseEvent>(skill);
					queue->Publish(e);
				}
			}

			if (input->IsKeyTriggered(KEY_D))
			{

				PRINT("KEY D: SKILL USE");
				// Create UseSkillEvent
				// 
				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				if (playerSkillComponent->GetCurrentSkill() != nullptr)
				{
					auto e = MemoryManager::Make_shared<SkillUseEvent>(playerSkillComponent->GetCurrentSkill());
					queue->Publish(e);
				}
			}

			if (input->IsKeyTriggered(KEY_U))
			{
				PRINT("KEY U: SKILL UPGRADE");
				// TODO: create a multiple projectile support skill - DONE
				//		 create skill upgrade event with active skill id and support skill - DONE
				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				//auto player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				if (playerSkillComponent->GetCurrentSkill() != nullptr)
				{
					std::shared_ptr<MultipleProjectile> mp = MemoryManager::Make_shared<MultipleProjectile>(SupportSkillType::MULTIPLE_PROJECTILE);
					auto e = MemoryManager::Make_shared<SkillUpgradeEvent>(playerSkillComponent->GetCurrentSkill()->GetActiveSkillType(), mp);
					queue->Publish(e);
				}
			}

			if (input->IsKeyTriggered(KEY_G))
			{
				PRINT("KEY I: ACTIVATE SKILL ICE BALL");


				// TODO: skill add event

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				std::shared_ptr<IceballAttack> iceballAttack = MemoryManager::Make_shared<IceballAttack>(ActiveSkillType::ICE_BALL);
				auto e = MemoryManager::Make_shared<SkillAdditionEvent>(iceballAttack);
				queue->Publish(e);
			}

			if (input->IsKeyTriggered(KEY_F))
			{
				PRINT("KEY F: ACTIVATE SKILL FIRE BALL");


				// TODO: skill add event

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				std::shared_ptr<FireballAttack> fireballAttack = MemoryManager::Make_shared<FireballAttack>(ActiveSkillType::FIRE_BALL);
				auto e = MemoryManager::Make_shared<SkillAdditionEvent>(fireballAttack);
				queue->Publish(e);
			}

			if (input->IsKeyTriggered(KEY_S))
			{
				PRINT("KEY A: SWITCH BETWEEN FIRE AND ICE");

				// TODO: skill add event

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				auto e = MemoryManager::Make_shared<SkillActivationEvent>();
				queue->Publish(e);
			}

			if (input->IsKeyTriggered(KEY_R))
			{
				PRINT("KEY R: REMOVE SKILL FIRE BALL");

				ComponentDecorator<PlayerSkillComponent, GameObjectType> playerSkillComponent;
				auto player = m_registeredEntities.at(0);
				m_parentWorld->Unpack(player, playerSkillComponent);

				//std::shared_ptr<FireballAttack> fireballAttack = MemoryManager::Make_shared<FireballAttack>();
				auto e = MemoryManager::Make_shared<SkillRemovalEvent>(playerSkillComponent->GetCurrentSkill()->GetId());
				queue->Publish(e);
			}


			// (Demo) Spawn tower by pressing keys
			if (input->IsKeyTriggered(KEY_SPACE))
			{
				PRINT("SPACE");
				if (!GameLevelMapManager::GetInstance()->IsLevelStarted())
				{
					GameLevelMapManager::GetInstance()->StartLevel();
				}
			}

			if (GameLevelMapManager::GetInstance()->IsInGame())
			{
				if (input->IsKeyTriggered(KEY_I)) WidgetManager::GetInstance()->GetInventoryMenu().SetVisible(!WidgetManager::GetInstance()->GetInventoryMenu().GetVisible());
				if (input->IsKeyTriggered(KEY_P)) WidgetManager::GetInstance()->GetShopMenu().SetVisible(!WidgetManager::GetInstance()->GetShopMenu().GetVisible());
				if (input->IsKeyTriggered(KEY_ESCAPE))
				{
					WidgetManager::GetInstance()->GetPauseMenu().SetVisible(!WidgetManager::GetInstance()->GetPauseMenu().GetVisible());
					m_parentWorld->SetPause(WidgetManager::GetInstance()->GetPauseMenu().GetVisible());
				}
			}
		}

		bool HandleMouseCondition_Move()
		{
			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<BodyCom, GameObjectType> mouseBodyCom;
			m_parentWorld->Unpack(mouse, mouseBodyCom);

			bool couldMove = true;
			switch (mouseBodyCom->GetOtherEntity().m_type)
			{

			case GameObjectType::TOWER_BUILD: case GameObjectType::TOWER_FIRE: case GameObjectType::TOWER_ICE: case GameObjectType::TOWER_LIGHTNING:
				couldMove = false;
				break;
			default:
				break;
			}
			return couldMove;
		}

		void HandleMouseAction_LeftPressed()
		{
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

		void HandleMouseAction_LeftTriggered()
		{
			auto mouse = m_parentWorld->GetAllEntityWithType(GameObjectType::MOUSE)[0];
			ComponentDecorator<BodyCom, GameObjectType> mouseBodyCom;
			m_parentWorld->Unpack(mouse, mouseBodyCom);

			switch (mouseBodyCom->GetOtherEntity().m_type)
			{
			case GameObjectType::TOWER_BUILD:
			{
				auto tower = mouseBodyCom->GetOtherEntity();
				ComponentDecorator<SpriteCom, GameObjectType> towerSprite;
				ComponentDecorator<ChildrenCom<GameObjectType>, GameObjectType> towerChildren;
				m_parentWorld->Unpack(tower, towerSprite);
				m_parentWorld->Unpack(tower, towerChildren);
				if (towerSprite->GetTextureName().compare("TowerHammer_On") == 0)
				{
					towerSprite->SetTexture("TowerHammer_Off");
					for (auto& _tower : towerChildren->GetEntities())
					{
						ComponentDecorator<ActiveCom, GameObjectType> active;
						ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
						m_parentWorld->Unpack(_tower, active);
						m_parentWorld->Unpack(_tower, coolDownController);
						active->SetActive(true);
						coolDownController->SetFreeze(true);
					}
				}
				else
				{
					towerSprite->SetTexture("TowerHammer_On");
					for (auto& _tower : towerChildren->GetEntities())
					{
						ComponentDecorator<ActiveCom, GameObjectType> active;
						m_parentWorld->Unpack(_tower, active);
						active->SetActive(false);
					}
				}
			}
			break;
			case GameObjectType::TOWER_FIRE: case GameObjectType::TOWER_ICE: case GameObjectType::TOWER_LIGHTNING:
			{
				auto _tower = mouseBodyCom->GetOtherEntity();
				ComponentDecorator<BodyCom, GameObjectType> transform;
				ComponentDecorator<CoolDownCom, GameObjectType> coolDownController;
				ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> ownership;
				m_parentWorld->Unpack(_tower, transform);
				m_parentWorld->Unpack(_tower, coolDownController);
				m_parentWorld->Unpack(_tower, ownership);

				// Unfreeze tower as a way to show it has been enabled.
				// Do not re-enabled unfreezed tower
				if (!coolDownController->IsFreezed())
				{
					break;
				}
				coolDownController->SetFreeze(false);
				auto tower = ownership->GetEntity();
				ComponentDecorator<BodyCom, GameObjectType> towerBody;
				ComponentDecorator<ActiveCom, GameObjectType> towerActive;
				ComponentDecorator<ChildrenCom<GameObjectType>, GameObjectType> towerChildren;
				m_parentWorld->Unpack(tower, towerBody);

				// Simply swap the position of build tower and this tower
				auto towerPos = towerBody->GetPos();
				towerBody->SetPos(transform->GetPos());
				transform->SetPos(towerPos);
				m_parentWorld->Unpack(tower, towerActive);

				// Deactivate other towers
				towerActive->SetActive(false);
				m_parentWorld->Unpack(tower, towerChildren);

				for (auto& child : towerChildren->GetEntities())
				{
					if (child.m_type != _tower.m_type)
					{
						ComponentDecorator<ActiveCom, GameObjectType> active;
						m_parentWorld->Unpack(child, active);
						active->SetActive(false);
					}
				}
			}
			break;
			default:
				break;
			}
		}

		void HandlePlayerMovement(double dt)
		{
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
				animation->SetCurrentAnimationState("Idle");
				return;
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
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
			if (dt)
			{
				transform->SetRotation(angle);
			}
			// 2. Move
			body->SetVelocity(glm::normalize(delta) * m_speed);
			animation->SetCurrentAnimationState("Move");

			// 3. Play sound
			if (dt)
			{
				auto e = MemoryManager::Make_shared<SoundEvent>("footstep02");
				queue->Publish(e);
			}
		}
	};
}