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

namespace gswy
{

	class MainMenuControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		bool m_bIsMainMenuLoaded = { false };
	public:
		MainMenuControllerComSys() {
		}

		virtual void Init() override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<MainMenuControllerComSys>(this, EventType::LOAD_MAIN_MENU, &MainMenuControllerComSys::OnLoadMainMenuWorld);
		}


		void OnLoadMainMenuWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			m_bIsMainMenuLoaded = true;
		}

		virtual void Update(double dt) override {	
			if (!m_bIsMainMenuLoaded)
			{
				auto input = InputManager::GetInstance();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();

				// Escape the start up screen by pressing ESC
				if (input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || input->IsKeyTriggered(KEY_ENTER) || input->IsKeyTriggered(KEY_SPACE) || input->IsKeyTriggered(KEY_ESCAPE))
				{
					auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
					queue->Publish(_e1,0.05);
				}
			}
			else
			{
				ProcessConstantInput();
			}
		}

		void ProcessConstantInput()
		{
			auto credit_page = m_parentWorld->GetAllEntityWithType(GameObjectType::CREDITS);
			if (!credit_page.empty())
			{
				auto input = InputManager::GetInstance();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				if (input->IsKeyTriggered(KEY_ESCAPE) || input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT) || input->IsKeyTriggered(KEY_SPACE))
				{
					auto e = MemoryManager::Make_shared<GCEvent>(credit_page[0]);
					queue->Publish(e);
					// Set widget
					{
						WidgetManager* manager = WidgetManager::GetInstance();
						manager->GetMainMenu().SetVisible(true);
					}
				}
			}
		}
	};
}