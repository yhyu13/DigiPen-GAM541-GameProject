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
#include "engine/audio/AudioManager.h"
#include "engine/input/InputManager.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class PlayerAnimationControllerComSys : public BaseComponentSystem<GameObjectType> {
		std::string m_currentAnimationState = { "" };
		std::string m_pendingAnimationState = { "" };
		bool m_bIsReady = { false };
		double m_delay = { 0.0 };
		std::map<std::string, std::map<std::string, bool>> m_animationValidGraph;
		std::map<std::string, std::map<std::string, double>> m_animationDelayGraph;

	public:
		PlayerAnimationControllerComSys() {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<PlayerAnimationControllerComSys>(this, EventType::PLAYER_SET_PENDING_ANIMATION, &PlayerAnimationControllerComSys::OnSetPending);
			queue->Subscribe<PlayerAnimationControllerComSys>(this, EventType::PLAYER_READY_TO_CHANGE_ANIMATION, &PlayerAnimationControllerComSys::OnSetReady);
		}

		virtual void Init() override
		{
			// Initialize animation validation graph
			m_animationValidGraph["Idle"]["Move"] = true;
			m_animationValidGraph["Move"]["Idle"] = true;
			m_animationValidGraph["Idle"]["RazerAttack"] = true;
			m_animationValidGraph["RazerAttack"]["Idle"] = true;
			m_animationValidGraph["Move"]["RazerAttack"] = true;
			m_animationValidGraph["RazerAttack"]["Move"] = true;

			// Initialize animation delay graph
			m_animationDelayGraph["RazerAttack"]["Idle"] = 1.0;
			m_animationDelayGraph["RazerAttack"]["Move"] = 1.0;
		}

		virtual void Update(double dt) override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
		
			if (m_delay -= dt < 0)
			{
				m_delay = 0;
			}

			if (m_pendingAnimationState != "" && m_delay <= 0)
			{
				if (m_currentAnimationState == "" || m_animationValidGraph[m_currentAnimationState][m_pendingAnimationState])
				{
					auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
					auto anim = GetComponent<AnimationCom>(entity);
					anim->SetCurrentAnimationState(m_pendingAnimationState);
					m_delay = m_animationDelayGraph[m_currentAnimationState][m_pendingAnimationState];
					m_currentAnimationState = m_pendingAnimationState;
					m_pendingAnimationState = "";
				}
				else
				{
					// TODO : Engine exception
					// throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Switch animation state from " + str2wstr(m_currentAnimationState) + L" to " + str2wstr(m_pendingAnimationState) + L" is not valid!");
				}
				
			}
		}

		void OnSetPending(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<PlayerSetPendingAnimationEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

				if (event->m_force)
				{
					m_pendingAnimationState = event->m_name;
				}
				else if (m_pendingAnimationState == "")
				{
					m_pendingAnimationState = event->m_name;
				}
			}
		}

		void OnSetReady(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<PlayerSetReadyToChangeAnimationEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				m_bIsReady = event->m_bIsReady;
			}
		}
	};
}