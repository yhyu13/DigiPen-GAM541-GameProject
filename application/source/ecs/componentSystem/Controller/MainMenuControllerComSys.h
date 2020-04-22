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
#include "HowToPlayState.h"

namespace gswy
{
	class MainMenuControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		bool m_cheatEnabled = { false };
		HowToPlayState m_howToPlayState = { HowToPlayState::HOW_TO_PLAY_1 };

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
			queue->Subscribe<MainMenuControllerComSys>(this, EventType::LOAD_HOW_TO_PLAY, &MainMenuControllerComSys::OnLoadHowToPlay);
		}

		void OnLoadHowToPlay(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<LoadHowToPlayEvent>(e))
			{
				m_howToPlayState = HowToPlayState::HOW_TO_PLAY_1;
			}
		}

		virtual void Update(double dt) override {
			SyncRegisteredEntities();
			{
				ProcessConstantInput();
			}
		}

		void ProcessConstantInput()
		{
			auto input = InputManager::GetInstance();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

			// Process showing and hiding of the game credit page
			auto credit = m_parentWorld->GetAllEntityWithType(GameObjectType::CREDITS);
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
						WidgetManager::GetInstance()->GetMainMenu().SetVisible(true);
					}
				}
				return;
			}

			auto how_to_play = m_parentWorld->GetAllEntityWithType(GameObjectType::HOW_TO_PLAY);
			if (!how_to_play.empty())
			{
				auto entity = how_to_play[0];
				if (input->IsKeyTriggered(KEY_ESCAPE))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(entity);
					queue->Publish(e);
					// Set main menu back to visible
					{
						WidgetManager::GetInstance()->GetMainMenu().SetVisible(true);
					}
				}
				else
				{
					switch (m_howToPlayState)
					{
					case HowToPlayState::HOW_TO_PLAY_1:
					{
						if (input->IsKeyTriggered(KEY_DOWN) || input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
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
						if (input->IsKeyTriggered(KEY_UP) || input->IsMouseButtonTriggered(MOUSE_BUTTON_RIGHT))
						{
							ComponentDecorator<SpriteCom, GameObjectType> spriteCom;
							m_parentWorld->Unpack(entity, spriteCom);
							auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_1");
							auto howToPlaySprite = spriteCom->Get();
							howToPlaySprite->SetSpriteTexture(howToPlayTexture);
							m_howToPlayState = HowToPlayState::HOW_TO_PLAY_1;
						}
						else if (input->IsKeyTriggered(KEY_DOWN) || input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
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
						if (input->IsKeyTriggered(KEY_UP) || input->IsMouseButtonTriggered(MOUSE_BUTTON_RIGHT))
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
		}
	};
}