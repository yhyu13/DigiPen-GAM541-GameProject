/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/03/2020
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
		CREDIT,
		END
	};

	enum class HowToPlayState
	{
		HOW_TO_PLAY_1,
		HOW_TO_PLAY_2,
		HOW_TO_PLAY_3
	};

	class MainMenuControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		bool m_cheatEnabled = { false };
		bool m_bIsMainMenuLoaded = { false };
		SplashScreenState m_loadState = { SplashScreenState::DIGIPEN_LOGO };
		HowToPlayState m_howToPlayState = { HowToPlayState::HOW_TO_PLAY_1 };
		bool m_howToPlayInitialized = { false };

	public:
		MainMenuControllerComSys() {
		}

		virtual void Init() override
		{
			Json::Value engineConfiguration;
			std::ifstream file("./asset/engine-configuration/engine-config.json", std::ifstream::binary);
			file >> engineConfiguration;
			file.close();
			m_cheatEnabled = engineConfiguration["enable-cheat"].asBool();

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<MainMenuControllerComSys>(this, EventType::LOAD_MAIN_MENU, &MainMenuControllerComSys::OnLoadMainMenuWorld);
			queue->Subscribe<MainMenuControllerComSys>(this, EventType::ON_SPLASH_STATE_CHANGE, &MainMenuControllerComSys::OnSplashStateChange);
			queue->Subscribe<MainMenuControllerComSys>(this, EventType::LOAD_HOW_TO_PLAY, &MainMenuControllerComSys::OnLoadHowToPlay);
		}

		void OnLoadHowToPlay(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			APP_CRITICAL("HOW TO PLAY LOAD EVENT");
			if (!m_howToPlayInitialized)
			{
				auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_1");
				auto m_HowToPlayLogo = MemoryManager::Make_shared<EntityDecorator<GameObjectType>>(m_parentWorld->GenerateEntity(GameObjectType::HOW_TO_PLAY));
				auto howToPlayLogoActive = ActiveCom(true);
				m_HowToPlayLogo->AddComponent(howToPlayLogoActive);
				auto howToPlaySpriteCom = SpriteCom();
				auto howToPlaySprite = howToPlaySpriteCom.Get();
				howToPlaySprite->SetSpriteTexture(howToPlayTexture);
				howToPlaySprite->SetSpriteScale(vec2(3.6f, 3.6f / howToPlayTexture->GetWidth() * howToPlayTexture->GetHeight()));
				howToPlaySprite->SetSpritePosition(vec3(0));
				m_HowToPlayLogo->AddComponent(howToPlaySpriteCom);
				auto howToPlayTransform = TransformCom(0, 0, Z_ORDER(2500));
				m_HowToPlayLogo->AddComponent(howToPlayTransform);
				m_howToPlayInitialized = true;

				WidgetManager::GetInstance()->GetMainMenu().SetVisible(false);
			}
		}

		void OnLoadMainMenuWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			m_bIsMainMenuLoaded = true;
		}

		void OnSplashStateChange(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<OnSplashStateChangeEvent>(e))
			{
				m_loadState = (SplashScreenState)event->m_state;
			}
		}

		virtual void Update(double dt) override {	
			if (!m_bIsMainMenuLoaded)
			{
				auto input = InputManager::GetInstance();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

				// Escape the start up screen by pressing ESC, ENTER, SPACE, and MLB
				if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || input->IsKeyTriggered(KEY_ENTER) || input->IsKeyTriggered(KEY_SPACE) || input->IsKeyTriggered(KEY_ESCAPE))
				{

					auto howToPlay = m_parentWorld->GetAllEntityWithType(GameObjectType::HOW_TO_PLAY);
					if (!howToPlay.empty())
					{
						auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
						auto e = MemoryManager::Make_shared<GCEvent>(howToPlay[0]);
						queue->Publish(e);
						APP_CRITICAL("HOW TO PLAY DELETED!!");
						m_howToPlayInitialized = false;
					}

					auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
					// Pulish event in the next frame
					queue->Publish(_e1,dt);
					m_bIsMainMenuLoaded = true;
					return;
				}

				if (input->IsKeyTriggered(KEY_TAB) || input->IsKeyTriggered(KEY_S))
				{
					switch (m_loadState)
					{
					case SplashScreenState::DIGIPEN_LOGO:
					{
						queue->RemoveDelayedEvent(EventType::FADE);
						queue->RemoveDelayedEvent(EventType::LOAD_TEAM_LOGO);

						auto m_DigipenLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::DIGIPEN_LOGO)[0];
						auto _e = MemoryManager::Make_shared<FadeEvent>(m_DigipenLogo, 1.f, -0.5f, 0.1f, EventType::GC);
						queue->Publish(_e);

						auto teamLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_TEAM_LOGO);
						queue->Publish(teamLogoEvent);
					}
					break;

					case SplashScreenState::TEAM_LOGO:
					{
						queue->RemoveDelayedEvent(EventType::FADE);
						queue->RemoveDelayedEvent(EventType::LOAD_GAME_LOGO);

						auto m_teamLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::TEAM_LOGO)[0];
						auto _e = MemoryManager::Make_shared<FadeEvent>(m_teamLogo, 1.f, -0.5f, 0.1f, EventType::GC);
						queue->Publish(_e);

						auto gameLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_GAME_LOGO);
						queue->Publish(gameLogoEvent);
					}
					break;

					case SplashScreenState::GAME_LOGO:
					{
						auto m_gameLogo = m_parentWorld->GetAllEntityWithType(GameObjectType::GAME_LOGO)[0];
						auto _e = MemoryManager::Make_shared<FadeEvent>(m_gameLogo, 1.f, -0.5f, 0.1f, EventType::GC);
						queue->Publish(_e);

						auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
						queue->Publish(_e1, dt);
						m_bIsMainMenuLoaded = true;
					}
					break;
					}
				}

				if (m_howToPlayInitialized)
				{
					Entity<GameObjectType> entity = m_parentWorld->GetAllEntityWithType(GameObjectType::HOW_TO_PLAY)[0];
					InputManager* input = InputManager::GetInstance();
					switch (m_howToPlayState)
					{
					case HowToPlayState::HOW_TO_PLAY_1:
						if (input->IsKeyTriggered(KEY_DOWN))
						{
							APP_CRITICAL("HOW TO PLAY 1 KEY DOWN");
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_2");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_2;
						}
						break;

					case HowToPlayState::HOW_TO_PLAY_2:

						if (input->IsKeyTriggered(KEY_UP))
						{
							APP_CRITICAL("HOW TO PLAY 2 KEY UP");
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_1");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_1;
						}
						else if (input->IsKeyTriggered(KEY_DOWN))
						{
							APP_CRITICAL("HOW TO PLAY 2 KEY DOWN");
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_3");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_3;
						}
						break;

					case HowToPlayState::HOW_TO_PLAY_3:
						if (input->IsKeyTriggered(KEY_UP))
						{
							APP_CRITICAL("HOW TO PLAY 3 KEY UP");
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_2");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_2;
						}
						break;
					}
				}
			}
			else
			{
				ProcessConstantInput();
			}
		}

		void ProcessConstantInput()
		{
			// Process showing and hiding of the game credit page
			auto credit_page = m_parentWorld->GetAllEntityWithType(GameObjectType::CREDITS);
			if (!credit_page.empty())
			{
				auto credit = credit_page[0];
				auto input = InputManager::GetInstance();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				if (input->IsKeyTriggered(KEY_ESCAPE) || input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || input->IsKeyTriggered(KEY_SPACE))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(credit);
					queue->Publish(e);
					// Set main menu back to visible
					{
						WidgetManager::GetInstance()->GetMainMenu().SetVisible(true);
					}

				}
			}
		}
	};
}