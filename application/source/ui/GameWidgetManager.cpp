/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: GameWidgetManager.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 03/12/2020
- End Header ----------------------------*/

#include "GameWidgetManager.h"
#include "inventory-manager/InventoryManager.h"
#include "tilemap/GameLevelMapManager.h"
#include "skill-manager/SkillManager.h"
#include "engine/audio/AudioManager.h"
#include "ecs/componentSystem/Audio/SoundComSys.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

namespace gswy {

	WidgetManager* WidgetManager::s_instance = 0;

	ImGuiWindowFlags popupFlag = { ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize };
	ImGuiWindowFlags menuFlag = { ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse };

	WidgetManager* WidgetManager::GetInstance()
	{
		if (!s_instance) s_instance = new WidgetManager();
		return s_instance;
	}

	WidgetManager::WidgetManager()
	{
		m_Hud.manager = this;
		m_MainMenu.manager = this;
		m_PauseMenu.manager = this;
		m_ShopMenu.manager = this;
		m_InventoryMenu.manager = this;

		LoadWidget("./asset/archetypes/Widget.json");
		m_MainMenu.Init();
		m_PauseMenu.Init();
		m_OptionMenu.Init();
	}

	void WidgetManager::RenderUI()
	{
		// Update widgets
		{
			m_OptionMenu.Update();
		}
		
		// Render widgets
		{
			if (m_Hud.GetVisible())              m_Hud.Render();
			if (m_MainMenu.GetVisible() && !m_OptionMenu.GetVisible())
				m_MainMenu.Render();
			if (m_PauseMenu.GetVisible() && !m_OptionMenu.GetVisible())
				m_PauseMenu.Render();
			if (m_OptionMenu.GetVisible())		 m_OptionMenu.Render();
			if (m_ShopMenu.GetVisible())         m_ShopMenu.Render();
			if (m_InventoryMenu.GetVisible())    m_InventoryMenu.Render();
		}
	}

	void WidgetManager::PushPopModalStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(255, 137, 20));
	}

	void WidgetManager::PopPopupModalStyle()
	{
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(3);
	}

	void WidgetManager::PushTabStyle()
	{
		ImGui::PushStyleColor(ImGuiCol_TabHovered, (ImVec4)ImColor::ImColor(0, 255, 43));
		ImGui::PushStyleColor(ImGuiCol_TabActive, (ImVec4)ImColor::ImColor(0, 230, 39));
		ImGui::PushStyleColor(ImGuiCol_Tab, (ImVec4)ImColor::ImColor(0, 184, 31));
	}

	void WidgetManager::PopTabStyle()
	{
		ImGui::PopStyleColor(3);
	}

	void WidgetManager::LoadWidget(const std::string& filepath)
	{
		Json::Value root;
		std::ifstream file(filepath, std::ifstream::binary);
		file >> root;
		Json::Value widgets = root["Widget"];
		for (int i = 0; i < widgets.size(); ++i)
		{
			Json::Value widgetData = widgets[i];
			GetMainMenu().m_TexturePath_NewGame = widgetData["NewGame"].asString();
			GetMainMenu().m_TexturePath_HowToPlay = widgetData["HowToPlay"].asString();
			GetMainMenu().m_TexturePath_Option = widgetData["Option"].asString();
			GetMainMenu().m_TexturePath_Credits = widgetData["Credits"].asString();
			GetMainMenu().m_TexturePath_QuitGame = widgetData["QuitGame"].asString();
			GetPauseMenu().m_TexturePath_ResumeGame = widgetData["ResumeGame"].asString();
			GetPauseMenu().m_TexturePath_MainMenu = widgetData["MainMenu"].asString();
		}
		auto styles = root["styles"];
		{
			m_Hud.SetStyle(styles["HUD"][0].asInt(), styles["HUD"][1].asInt(), styles["HUD"][2].asInt(), styles["HUD"][3].asInt());
			m_MainMenu.SetStyle(styles["MainMenu"][0].asInt(), styles["MainMenu"][1].asInt(), styles["MainMenu"][2].asInt(), styles["MainMenu"][3].asInt());
			m_PauseMenu.SetStyle(styles["PauseMenu"][0].asInt(), styles["PauseMenu"][1].asInt(), styles["PauseMenu"][2].asInt(), styles["PauseMenu"][3].asInt());
			m_OptionMenu.SetStyle(styles["OptionMenu"][0].asInt(), styles["OptionMenu"][1].asInt(), styles["OptionMenu"][2].asInt(), styles["OptionMenu"][3].asInt());
			m_ShopMenu.SetStyle(styles["ShopMenu"][0].asInt(), styles["ShopMenu"][1].asInt(), styles["ShopMenu"][2].asInt(), styles["ShopMenu"][3].asInt());
			m_InventoryMenu.SetStyle(styles["InventoryMenu"][0].asInt(), styles["InventoryMenu"][1].asInt(), styles["InventoryMenu"][2].asInt(), styles["InventoryMenu"][3].asInt());
		}
	}

	void MainMenu::Render()
	{
		ImVec2 windowsize = ImVec2(GetWindowSize_X(), GetWindowSize_Y());
		ImVec2 mainMenuWindowSize(m_MainMenu.x, m_MainMenu.y);
		ImGui::SetNextWindowSize(mainMenuWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - mainMenuWindowSize[0] / 2, windowsize[1] / 2 - mainMenuWindowSize[1] / 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("A new world", false, menuFlag);
		float f = 0.0;
		ImGui::SetWindowFontScale(1.0);
		if (ImGui::ImageButton((void*)m_Texture_NewGame->GetRendererID(), ImVec2(480, 100),ImVec2(0,1), ImVec2(1,0), 0, ImVec4(0,0,0,1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("New Game");
		}
		if (ImGui::ImageButton((void*)m_Texture_HowToPlay->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("How To Play");
		}
		if (ImGui::ImageButton((void*)m_Texture_Option->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			//manager->InvokeButton("Option");
			manager->GetOptionMenu().SetVisible(true);
			manager->GetOptionMenu().SetCallFromMainMenu(true);
		}
		if (ImGui::ImageButton((void*)m_Texture_Credits->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Credits");
		}
		if (ImGui::ImageButton((void*)m_Texture_QuitGame->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			ImGui::OpenPopup("Quit?");
		}
		if (ImGui::BeginPopupModal("Quit?", NULL, popupFlag))
		{
			ImGui::Text("Are you sure to quit game?\n");
			ImGui::Separator();
			WidgetManager::GetInstance()->PushPopModalStyle();
			if (ImGui::Button("Yes", ImVec2(120, 0))) 
			{ 
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup(); 
				manager->InvokeButton("Exit");
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0))) 
			{ 
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup();
			}
			WidgetManager::GetInstance()->PopPopupModalStyle();
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void PauseMenu::Render()
	{
		ImVec2 windowsize = ImVec2(GetWindowSize_X(), GetWindowSize_Y());
		ImVec2 pauseMenuWindowSize(m_PauseMenuWindowSize.x, m_PauseMenuWindowSize.y);
		ImGui::SetNextWindowSize(pauseMenuWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - pauseMenuWindowSize[0] / 2, windowsize[1] / 2 - pauseMenuWindowSize[1] / 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));
		ImGui::Begin("A new world", false, menuFlag);
		ImGui::SetWindowFontScale(1.0);
		if (ImGui::ImageButton((void*)m_Texture_ResumeGame->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Resume Game");
		}
		if (ImGui::ImageButton((void*)WidgetManager::GetInstance()->GetMainMenu().m_Texture_HowToPlay->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("How To Play");
		}
		if (ImGui::ImageButton((void*)WidgetManager::GetInstance()->GetMainMenu().m_Texture_Option->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			//manager->InvokeButton("Option");
			manager->GetOptionMenu().SetVisible(true);
			manager->GetOptionMenu().SetCallFromMainMenu(false);
		}
		if (ImGui::ImageButton((void*)m_Texture_MainMenu->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			ImGui::OpenPopup("Main menu?");
		}
		if (ImGui::BeginPopupModal("Main menu?", NULL, popupFlag))
		{
			ImGui::Text("Are you sure to go back?\n");
			ImGui::Separator();
			WidgetManager::GetInstance()->PushPopModalStyle();
			if (ImGui::Button("Yes", ImVec2(120, 0)))
			{
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup();
				manager->InvokeButton("Main Menu");
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup();
			}
			WidgetManager::GetInstance()->PopPopupModalStyle();
			ImGui::EndPopup();
		}
		if (ImGui::ImageButton((void*)WidgetManager::GetInstance()->GetMainMenu().m_Texture_QuitGame->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			ImGui::OpenPopup("Quit?");
		}
		if (ImGui::BeginPopupModal("Quit?", NULL, popupFlag))
		{
			ImGui::Text("Are you sure to quit game?\n");
			ImGui::Separator();
			WidgetManager::GetInstance()->PushPopModalStyle();
			if (ImGui::Button("Yes", ImVec2(120, 0)))
			{
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup();
				manager->InvokeButton("Exit");
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0)))
			{
				AudioManager::GetInstance()->PlaySound("click_sound");
				ImGui::CloseCurrentPopup();
			}
			WidgetManager::GetInstance()->PopPopupModalStyle();
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
	}

	void OptionMenu::Render()
	{
		ImVec2 windowsize = ImVec2(GetWindowSize_X(), GetWindowSize_Y());
		ImVec2 optionWindowSize(500, 535);
		ImGui::SetNextWindowSize(optionWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - optionWindowSize[0] / 2, windowsize[1] / 2 - optionWindowSize[1] / 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::Begin("Option", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::Dummy({ optionWindowSize.x, 30 });
			Engine& engine = Engine::Get();

			int selectBarWidth = 100;
			//Full Screen
			ImGui::SetCursorPosX(30);
			ImGui::Text("Full Screen");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(selectBarWidth);
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - selectBarWidth * 2);
			static int fullScreen = engine.GetWindow().GetWindowProperties().IsFullScreen;
			if (ImGui::Combo("##FullScreen", &fullScreen, "Off\0On\0"))
			{
				engine.GetWindow().ToggleFullScreen(fullScreen);
				WidgetManager::GetInstance()->InvokeButton("Full Screen");
			}
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Dummy({ optionWindowSize.x, 30 });

			//V-Sync
			ImGui::SetCursorPosX(30);
			ImGui::Text("V-Sync");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(selectBarWidth);
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - selectBarWidth * 2);
			static int vsync = engine.GetWindow().GetWindowProperties().IsVSync;
			if (ImGui::Combo("##V-Sync", &vsync, "Off\0On\0"))
			{
				engine.GetWindow().SetVSync(vsync);
			}
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Dummy({ optionWindowSize.x, 30 });

			//Mute BGM
			ImGui::SetCursorPosX(30);
			ImGui::Text("Mute BGM");
			ImGui::SameLine();
			
			ImGui::SetNextItemWidth(selectBarWidth);
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - selectBarWidth * 2);
			static int muteMusic = m_MuteMusic;
			if (ImGui::Combo("##Mute BGM", &muteMusic, "Off\0On\0"))
			{
				m_MuteMusic = muteMusic;
			}
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::Dummy({ optionWindowSize.x, 30 });

			//Mute SFX
			ImGui::SetCursorPosX(30);
			ImGui::Text("Mute SFX");
			ImGui::SameLine();

			ImGui::SetNextItemWidth(selectBarWidth);
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - selectBarWidth * 2);
			static int muteSFX = m_MuteSFX;
			if (ImGui::Combo("##Mute SFX", &muteSFX, "Off\0On\0"))
			{
				m_MuteSFX = muteSFX;
			}
			ImGui::NewLine();
			ImGui::Separator();

			//Back Button
			ImGui::SetCursorPos({ 30,450 });
			if (ImGui::Button("Back", ImVec2(100,30)) || ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Escape)))
			{
				WidgetManager::GetInstance()->GetOptionMenu().SetVisible(false);
				if(m_CallFromMainMenu)
					WidgetManager::GetInstance()->GetMainMenu().SetVisible(true);
				else
					WidgetManager::GetInstance()->GetPauseMenu().SetVisible(true);
			}
		}

		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(3);
	}

	void OptionMenu::Update()
	{
		if(m_MuteMusic)
			SoundManager::GetInstance()->CallForMuteBGM("Track_1", -100.0f);
		else
			SoundManager::GetInstance()->CallForMuteBGM("Track_1", 1.0f);
		SoundManager::GetInstance()->CallForMuteSFX(m_MuteSFX);
	}

	HUD::HUD() 
	{ 
		IsVisible = false; 
		m_TimerMin = 0; 
		m_TimerSec = 0; 
		m_Wave = 1; 
		m_Level = 1;
		m_Coins = 0;
		m_Progress = 0.5f;
		m_BaseHP = 0.5f;
		m_PlayerHP = 0.5f;
		m_FrameBuffer = FrameBuffer::Create(GetWindowSize_X(), GetWindowSize_Y());
		m_Texture_Coin = Texture2D::Create("./asset/Sprites/coin_ui.png");

		m_WaveWindowSize = { 80, 20 };
		m_BaseHPWindowSize = { 500, 40 };
		m_TimeWindowSize = { 60, 17 };
		m_CoinWindowSize = { 140, 70 };
		m_PlayerHPWindowSize = { 300, 40 };
	}

	void HUD::Render()
	{	
		//Text : Wave
		ImVec2 waveWindowSize = ImVec2(m_WaveWindowSize.x, m_WaveWindowSize.y);
		ImGui::SetNextWindowSize(waveWindowSize);
		ImVec2 waveWindowPos = ImVec2(GetWindowSize_X() / 2 - waveWindowSize[0] / 2, 12);
		ImGui::SetNextWindowPos(waveWindowPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Wave", false, menuFlag);
		ImGui::Text("Wave : %i", m_Wave);
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);

		//Progress Bar : Base HP
		ImVec2 BaseHPWindowSize = ImVec2(m_BaseHPWindowSize.x, m_BaseHPWindowSize.y);
		ImGui::SetNextWindowSize(BaseHPWindowSize);
		ImVec2 BaseHPWindowPos = ImVec2(GetWindowSize_X() / 2 - BaseHPWindowSize[0] / 2, waveWindowSize.y + waveWindowPos.y + 17);
		ImGui::SetNextWindowPos(BaseHPWindowPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Base HP", false, menuFlag);
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Base HP");
		ImGui::SameLine();
		ImGui::ProgressBarCustomColor(m_BaseHP, ImVec4((1.0f - m_BaseHP), m_BaseHP, 0, 1));
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);

		//Text : Timer
		ImVec2 timerWindowSize = ImVec2(m_TimeWindowSize.x, m_TimeWindowSize.y);
		ImGui::SetNextWindowSize(timerWindowSize);
		ImVec2 timerWindowPos = ImVec2(GetWindowSize_X() / 2 - timerWindowSize[0] / 2, BaseHPWindowSize.y + BaseHPWindowPos.y + 5);
		ImGui::SetNextWindowPos(timerWindowPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Timer", false, menuFlag);
		ImGui::Text("%i : %i", m_TimerMin, m_TimerSec);
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		
		//Text : Coin
		ImVec2 nextWindowSize = ImVec2(m_CoinWindowSize.x, m_CoinWindowSize.y);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(30, GetWindowSize_Y() - nextWindowSize[1] - 65));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Coin", false, menuFlag | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetCursorPosX(15);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),"%i", m_Coins);
		ImGui::Image((void*)m_Texture_Coin->GetRendererID(), ImVec2(40, 40));
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		
		//Progress Bar : Player HP
		nextWindowSize = ImVec2(m_PlayerHPWindowSize.x, m_PlayerHPWindowSize.y);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(100, GetWindowSize_Y() - nextWindowSize[1] - 70));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Player HP", false, menuFlag);
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Player HP");
		ImGui::SameLine();
		ImGui::ProgressBarCustomColor(m_PlayerHP, ImVec4((1.0f-m_PlayerHP), m_PlayerHP,0,1));
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void ShopMenu::Render()
	{
		// Insufficent coins pop up lambda function
		static void(*ShowPopup)() = []() {};

		ImVec2 shopWindowSize = ImVec2(m_ShopMenuWindowSize.x, m_ShopMenuWindowSize.y);
		ImGui::SetNextWindowSize(shopWindowSize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::Begin("Shop", false, menuFlag);
		if (ImGui::BeginTabBar("ShopTabBar"))
		{
			WidgetManager::GetInstance()->PushTabStyle();
			if (ImGui::BeginTabItem("ACTIVE"))
			{
				ImGui::Dummy(ImVec2(250, 15));
				auto items = InventoryManager::GetInstance()->GetActiveItems();
				auto begin = items.begin();
				auto end = items.end();
				for (auto it = begin; it != end; ++it)
				{
					//Query : have been purchased
					bool bPurchased = (*it)->m_purchased;
					int cost = (*it)->m_cost;
					
					//Query : color
					ImGui::PushStyleColor(ImGuiCol_Button, bPurchased ? (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f) : (ImVec4)ImColor::ImColor(1.0f, 1.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(1.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 1.0f));
					if (ImGui::Button(((*it)->m_type).c_str(), ImVec2(200, 50)))
					{
						ImGui::OpenPopup((*it)->m_type.c_str());

						//item hasn't been purchased
						if ((*it)->m_purchased)
						{
							
						}
						//item hasn been purchased
						else
						{
							
						}
					}
					ImGui::PopStyleColor(3);

					//Popup action to select
					if (ImGui::BeginPopup((*it)->m_type.c_str()))
					{
						//Item has not been purchased
						if (!bPurchased)
						{
							if (ImGui::Selectable("Purchase"))
							{
								if (GameLevelMapManager::GetInstance()->TrySpendCoins(cost))
								{
									AudioManager::GetInstance()->PlaySound("purchase_sound");
									InventoryManager::GetInstance()->PurchaseActiveItem((*it));
								}
								else
								{
									ShowPopup = []()
									{
										if (!ImGui::IsPopupOpen("popup"))
											ImGui::OpenPopup("popup");

										if (ImGui::BeginPopupModal("popup", NULL, popupFlag))
										{
											ImGui::Text("Insufficient coins!");
											WidgetManager::GetInstance()->PushPopModalStyle();
											if (ImGui::Button("Close", ImVec2(80, 0)))
											{
												ImGui::CloseCurrentPopup();
												ShowPopup = []() {};
											}
											WidgetManager::GetInstance()->PopPopupModalStyle();
											ImGui::EndPopup();
										}
									};
								}
							}
						}
						else
						{
							//Item has been purchased
							if (ImGui::BeginMenu("Install"))
							{
								if (ImGui::MenuItem("Bind to (Q)"))
								{
									AudioManager::GetInstance()->PlaySound("purchase_install_sound");
									InventoryManager::GetInstance()->Install(1, 1, (*it));
								}
								if (ImGui::MenuItem("Bind to (W)"))
								{
									AudioManager::GetInstance()->PlaySound("purchase_install_sound");
									InventoryManager::GetInstance()->Install(2, 1, (*it));
								}
								if (ImGui::MenuItem("Bind to (E)"))
								{
									AudioManager::GetInstance()->PlaySound("purchase_install_sound");
									InventoryManager::GetInstance()->Install(3, 1, (*it));
								}
								if (ImGui::MenuItem("Bind to (R)"))
								{
									AudioManager::GetInstance()->PlaySound("purchase_install_sound");
									InventoryManager::GetInstance()->Install(4, 1, (*it));
								}
								ImGui::EndMenu();
							}
							if (ImGui::BeginMenu("Remove"))
							{
								if (ImGui::MenuItem("Remove Q"))
								{
									SkillManager::GetInstance()->RemoveSkill(1, 1);
								}
								if (ImGui::MenuItem("Remove W"))
								{
									SkillManager::GetInstance()->RemoveSkill(2, 1);
								}
								if (ImGui::MenuItem("Remove E"))
								{
									SkillManager::GetInstance()->RemoveSkill(3, 1);
								}
								if (ImGui::MenuItem("Remove R"))
								{
									SkillManager::GetInstance()->RemoveSkill(4, 1);
								}
								ImGui::EndMenu();
							}
						}
						ImGui::EndPopup();
					}

					//Active Skill Tooltip
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();

						//Query : skill detail 
						for(auto s : (*it)->m_text)
							ImGui::Text("%s\n",s.c_str());
						ImGui::EndTooltip();
					}
					ImGui::NewLine();
				}
				ImGui::Dummy(ImVec2(250, 15));
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("SUPPORT"))
			{
				ImGui::Dummy(ImVec2(250, 15));
				auto supportItems = InventoryManager::GetInstance()->GetSupportItems();
				auto sbegin = supportItems.begin();
				auto send = supportItems.end();
				for (auto it = sbegin; it != send; ++it)
				{
					bool bSupportPurchased = (*it)->m_purchased;
					int cost = (*it)->m_cost;

					//Query : color
					ImGui::PushStyleColor(ImGuiCol_Button, bSupportPurchased ? (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f) : (ImVec4)ImColor::ImColor(1.0f, 1.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(1.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 1.0f));
					if (ImGui::Button(((*it)->m_type).c_str(), ImVec2(200, 50)))
					{
						ImGui::OpenPopup((*it)->m_type.c_str());
					}
					ImGui::PopStyleColor(3);

					if (ImGui::BeginPopup((*it)->m_type.c_str()))
					{
						//Item has not been purchased
						if (!bSupportPurchased)
						{
							if (ImGui::Selectable("Purchase"))
							{
								if (GameLevelMapManager::GetInstance()->TrySpendCoins(cost))
								{
									AudioManager::GetInstance()->PlaySound("purchase_sound");
									InventoryManager::GetInstance()->PurchaseSupportItem((*it));
								}
								else
								{
									ShowPopup = []()
									{
										if (!ImGui::IsPopupOpen("popup"))
											ImGui::OpenPopup("popup");
										if (ImGui::BeginPopupModal("popup", NULL, popupFlag))
										{
											ImGui::Text("Insufficient coins!");
											WidgetManager::GetInstance()->PushPopModalStyle();
											if (ImGui::Button("Close", ImVec2(80, 0)))
											{
												ImGui::CloseCurrentPopup();
												ShowPopup = []() {};
											}
											WidgetManager::GetInstance()->PopPopupModalStyle();
											ImGui::EndPopup();
										}
									};
								}
							}
						}
						else
						{
							if (ImGui::BeginMenu("SupportInstall"))
							{
								if (ImGui::BeginMenu("Bind to (Q)"))
								{
									if (ImGui::MenuItem("SUPPORT 1"))
									{
										InventoryManager::GetInstance()->Install(1, 2, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 2"))
									{
										InventoryManager::GetInstance()->Install(1, 3, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 3"))
									{
										InventoryManager::GetInstance()->Install(1, 4, (*it));
									}
									ImGui::EndMenu();
								}
								if (ImGui::BeginMenu("Bind to (W)"))
								{
									if (ImGui::MenuItem("SUPPORT 1"))
									{
										InventoryManager::GetInstance()->Install(2, 2, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 2"))
									{
										InventoryManager::GetInstance()->Install(2, 3, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 3"))
									{
										InventoryManager::GetInstance()->Install(2, 4, (*it));
									}
									ImGui::EndMenu();
								}
								if (ImGui::BeginMenu("Bind to (E)"))
								{
									if (ImGui::MenuItem("SUPPORT 1"))
									{
										InventoryManager::GetInstance()->Install(3, 2, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 2"))
									{
										InventoryManager::GetInstance()->Install(3, 3, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 3"))
									{
										InventoryManager::GetInstance()->Install(3, 4, (*it));
									}
									ImGui::EndMenu();
								}
								if (ImGui::BeginMenu("Bind to (R)"))
								{
									if (ImGui::MenuItem("SUPPORT 1"))
									{
										InventoryManager::GetInstance()->Install(4, 2, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 2"))
									{
										InventoryManager::GetInstance()->Install(4, 3, (*it));
									}
									if (ImGui::MenuItem("SUPPORT 3"))
									{
										InventoryManager::GetInstance()->Install(4, 4, (*it));
									}
									ImGui::EndMenu();
								}
								ImGui::EndMenu();
							}
						}
						ImGui::EndPopup();
					}

					//Support Skill Tooltip
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();

						//Query : skill detail 
						for (auto s : (*it)->m_text)
							ImGui::Text("%s\n", s.c_str());
						ImGui::EndTooltip();
					}
					ImGui::NewLine();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
			WidgetManager::GetInstance()->PopTabStyle();
		}

		ShowPopup();

		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void InventoryMenu::Render()
	{
		//New Inventory
		ImVec2 InventoryWindowSize = ImVec2(m_InventoryWindowSize.x, m_InventoryWindowSize.y);
		ImGui::SetNextWindowSize(InventoryWindowSize);
		ImGui::SetNextWindowPos(ImVec2(GetWindowSize_X() - InventoryWindowSize[0] - 80, GetWindowSize_Y() - InventoryWindowSize[1] - 20));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::Begin("Inventory", false, menuFlag);
		{
			//Active Skills
			if (SkillManager::GetInstance()->GetSkill(1, 1))
			{
				m_Texture_Acitve1 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(1, 1)->m_icon);
				ImGui::Image((void*)m_Texture_Acitve1->GetRendererID(), ImVec2(100, 100));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::Button("ACTIVE 1", ImVec2(94, 94));
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(2, 1))
			{
				m_Texture_Acitve2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(2, 1)->m_icon);
				ImGui::Image((void*)m_Texture_Acitve2->GetRendererID(), ImVec2(100, 100));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::Button("ACTIVE 2", ImVec2(94, 94));
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(3, 1))
			{
				m_Texture_Acitve3 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(3, 1)->m_icon);
				ImGui::Image((void*)m_Texture_Acitve3->GetRendererID(), ImVec2(100, 100));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::Button("ACTIVE 3", ImVec2(94, 94));
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(4, 1))
			{
				m_Texture_Acitve4 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(4, 1)->m_icon);
				ImGui::Image((void*)m_Texture_Acitve4->GetRendererID(), ImVec2(100, 100));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::Button("ACTIVE 4", ImVec2(94, 94));
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::NewLine();
		{
			//Support Skills
			if (SkillManager::GetInstance()->GetSkill(1, 2))
			{
				m_Texture_Support1_2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(1, 2)->m_icon);
				ImGui::SetCursorPosX(15);
				ImGui::Image((void*)m_Texture_Support1_2->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(15);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(1, 3))
			{
				m_Texture_Support1_3 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(1, 3)->m_icon);
				ImGui::SetCursorPosX(45);
				ImGui::Image((void*)m_Texture_Support1_3->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(45);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(1, 4))
			{
				m_Texture_Support1_4 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(1, 4)->m_icon);
				ImGui::SetCursorPosX(75);
				ImGui::Image((void*)m_Texture_Support1_4->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(75);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(2, 2))
			{
				m_Texture_Support2_2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(2, 2)->m_icon);
				ImGui::SetCursorPosX(120);
				ImGui::Image((void*)m_Texture_Support2_2->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::SetCursorPosX(105);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(2, 3))
			{
				m_Texture_Support2_3 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(2, 3)->m_icon);
				ImGui::SetCursorPosX(150);
				ImGui::Image((void*)m_Texture_Support2_3->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(150);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(2, 4))
			{
				m_Texture_Support2_4 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(2, 4)->m_icon);
				ImGui::SetCursorPosX(180);
				ImGui::Image((void*)m_Texture_Support2_4->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(180);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(3, 2))
			{
				m_Texture_Support3_2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(3, 2)->m_icon);
				ImGui::SetCursorPosX(225);
				ImGui::Image((void*)m_Texture_Support3_2->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::SetCursorPosX(202);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(3, 3))
			{
				m_Texture_Support3_3 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(3, 3)->m_icon);
				ImGui::SetCursorPosX(255);
				ImGui::Image((void*)m_Texture_Support3_3->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(255);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(3, 4))
			{
				m_Texture_Support3_4 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(3, 4)->m_icon);
				ImGui::SetCursorPosX(285);
				ImGui::Image((void*)m_Texture_Support3_4->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(285);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(4, 2))
			{
				m_Texture_Support4_2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(4, 2)->m_icon);
				ImGui::SetCursorPosX(330);
				ImGui::Image((void*)m_Texture_Support4_2->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::SetCursorPosX(299);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(4, 3))
			{
				m_Texture_Support4_3 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(4, 3)->m_icon);
				ImGui::SetCursorPosX(360);
				ImGui::Image((void*)m_Texture_Support4_3->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(360);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
			ImGui::SameLine();
			if (SkillManager::GetInstance()->GetSkill(4, 4))
			{
				m_Texture_Support4_4 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(4, 4)->m_icon);
				ImGui::SetCursorPosX(390);
				ImGui::Image((void*)m_Texture_Support4_4->GetRendererID(), ImVec2(30, 30));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(70, 70, 70));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(70, 70, 70));
				//ImGui::SetCursorPosX(390);
				ImGui::Button("", ImVec2(30, 30));
				ImGui::PopStyleColor(2);
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(4);
	}
}