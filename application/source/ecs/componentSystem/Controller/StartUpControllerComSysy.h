/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/15/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/input/InputManager.h"
#include "ui/GameWidgetManager.h"
#include "ecs/CustomEvents.h"
#include "ecs/components/ActiveCom.h"
#include "ecs/components/SpriteCom.h"

namespace gswy
{
	enum class SplashScreenState
	{
		DIGIPEN_LOGO,
		TEAM_LOGO,
		GAME_LOGO,
		END
	};

	class StartUpControllerComSysy : public BaseComponentSystem<GameObjectType> {
	private:
		SplashScreenState m_loadState = { SplashScreenState::DIGIPEN_LOGO };

	public:
		StartUpControllerComSysy() {
		}

		virtual void Init() override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<StartUpControllerComSysy>(this, EventType::ON_SPLASH_STATE_CHANGE, &StartUpControllerComSysy::OnSplashStateChange);
		}

		void OnSplashStateChange(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<OnSplashStateChangeEvent>(e))
			{
				m_loadState = (SplashScreenState)event->m_state;
			}
		}

		virtual void Update(double dt) override
		{
			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			// Escape the start up screen by pressing ESC, ENTER, SPACE, and MLB
			if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || input->IsKeyTriggered(KEY_ENTER) || input->IsKeyTriggered(KEY_SPACE) || input->IsKeyTriggered(KEY_ESCAPE))
			{
				auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
				// Pulish event in the next frame
				queue->Publish(_e1, dt);
				return;
			}

			if (input->IsKeyTriggered(KEY_TAB) || input->IsKeyTriggered(KEY_S))
			{
				switch (m_loadState)
				{
				case SplashScreenState::DIGIPEN_LOGO:
				{
					queue->RemoveDelayedEvent(EventType::FADE);
					queue->RemoveDelayedEvent(EventType::_FADE_ING);
					queue->RemoveDelayedEvent(EventType::_FADE_END);
					queue->RemoveDelayedEvent(EventType::LOAD_TEAM_LOGO);
					queue->RemoveDelayedEvent(EventType::LOAD_GAME_LOGO);
					queue->RemoveDelayedEvent(EventType::LOAD_MAIN_MENU);

					auto m_DigipenLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::DIGIPEN_LOGO);
					if (!m_DigipenLogo.empty())
					{
						auto _e = MemoryManager::Make_shared<GCEvent>(m_DigipenLogo[0]);
						queue->Publish(_e);
					}

					auto teamLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_TEAM_LOGO);
					queue->Publish(teamLogoEvent);

					auto gameLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_GAME_LOGO);
					queue->Publish(gameLogoEvent, 2.0f);
					auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
					queue->Publish(_e1, 6.0f);
				}
				break;

				case SplashScreenState::TEAM_LOGO:
				{
					queue->RemoveDelayedEvent(EventType::FADE);
					queue->RemoveDelayedEvent(EventType::_FADE_ING);
					queue->RemoveDelayedEvent(EventType::_FADE_END);
					queue->RemoveDelayedEvent(EventType::LOAD_GAME_LOGO);
					queue->RemoveDelayedEvent(EventType::LOAD_MAIN_MENU);

					auto m_teamLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::TEAM_LOGO);
					if (!m_teamLogo.empty())
					{
						auto _e = MemoryManager::Make_shared<GCEvent>(m_teamLogo[0]);
						queue->Publish(_e);
					}

					auto gameLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_GAME_LOGO);
					queue->Publish(gameLogoEvent);

					auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
					queue->Publish(_e1, 4.0f);
				}
				break;

				case SplashScreenState::GAME_LOGO:
				{
					auto m_gameLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::GAME_LOGO);
					if (!m_gameLogo.empty())
					{
						auto _e = MemoryManager::Make_shared<GCEvent>(m_gameLogo[0]);
						queue->Publish(_e);
					}

					auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
					queue->Publish(_e1, dt);
				}
				break;
				}
			}
		}
	};
}