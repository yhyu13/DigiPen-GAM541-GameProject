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
	ImGuiWindowFlags menuFlag = { ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration };

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
	}

	void WidgetManager::RenderUI()
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
			m_ShopMenu.SetStyle(styles["ShopMenu"][0].asInt(), styles["ShopMenu"][1].asInt(), styles["ShopMenu"][2].asInt(), styles["ShopMenu"][3].asInt());
			m_InventoryMenu.SetStyle(styles["InventoryMenu"][0].asInt(), styles["InventoryMenu"][1].asInt(), styles["InventoryMenu"][2].asInt(), styles["InventoryMenu"][3].asInt());
		}
	}

	void MainMenu::Render()
	{
		ImVec2 windowsize = ImVec2(m_WindowSize_X, m_WindowSize_Y);
		ImVec2 nextWindowSize(500, 535);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));
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
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void PauseMenu::Render()
	{
		ImVec2 windowsize = ImVec2(m_WindowSize_X, m_WindowSize_Y);
		ImVec2 nextWindowSize(500, 640);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));
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
		}
		if (ImGui::ImageButton((void*)WidgetManager::GetInstance()->GetMainMenu().m_Texture_Credits->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Credits");
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
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
	}

	void OptionMenu::Render()
	{
		ImVec2 windowsize = ImVec2(m_WindowSize_X, m_WindowSize_Y);
		ImVec2 OptionWindowSize(500, 535);
		ImGui::SetNextWindowSize(OptionWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - OptionWindowSize[0] / 2, windowsize[1] / 2 - OptionWindowSize[1] / 2));
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, { 255,137,20,255 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));

		ImGui::Begin("Option", &IsVisible, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::Dummy({ OptionWindowSize.x, 30 });
			ImGui::Checkbox("FullScreen", &m_FullScreen);
			if (m_FullScreen)
			{
				
			}
			ImGui::NewLine();
			ImGui::Dummy({ OptionWindowSize.x, 30 });
			ImGui::Checkbox("Mute Music", &m_MuteMusic);
			if (m_MuteMusic)
			{
				
			}
			ImGui::NewLine();
			ImGui::Dummy({ OptionWindowSize.x, 30 });
			ImGui::Checkbox("Mute All Audio", &m_MuteAllAudio);
			SoundManager::GetInstance()->CallForMute(m_MuteAllAudio);
		}

		ImGui::End();
		//ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
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
		m_FrameBuffer = FrameBuffer::Create(m_WindowSize_X, m_WindowSize_Y);
		m_Texture_Coin = Texture2D::Create("./asset/Sprites/coin_ui.png");
	}

	void HUD::Render()
	{	
		//Text : Wave
		ImVec2 waveWindowSize = ImVec2(80, 20);
		ImGui::SetNextWindowSize(waveWindowSize);
		ImVec2 waveWindowPos = ImVec2(m_WindowSize_X / 2 - waveWindowSize[0] / 2, 8);
		ImGui::SetNextWindowPos(waveWindowPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Wave", false, menuFlag);
		//ImFont* font = ImGui::GetFont();
		//float origScale = font->Scale;
		//font->Scale = 2.0f;
		//ImGui::PushFont(font);
		ImGui::Text("Wave : %i", m_Wave);
		//ImGui::PopFont();
		//font->Scale = origScale;
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);

		//Progress Bar : Base HP
		ImVec2 progressWindowSize = ImVec2(500, 40);
		ImGui::SetNextWindowSize(progressWindowSize);
		ImVec2 progressWindowPos = ImVec2(m_WindowSize_X / 2 - progressWindowSize[0] / 2, waveWindowSize.y + waveWindowPos.y + 17);
		ImGui::SetNextWindowPos(progressWindowPos);
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
		ImVec2 timerWindowSize = ImVec2(60, 17);
		ImGui::SetNextWindowSize(timerWindowSize);
		ImVec2 timerWindowPos = ImVec2(m_WindowSize_X / 2 - timerWindowSize[0] / 2, progressWindowSize.y + progressWindowPos.y + 5);
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
		ImVec2 nextWindowSize = ImVec2(140, 70);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(30, m_WindowSize_Y - nextWindowSize[1] - 65));
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
		nextWindowSize = ImVec2(300, 40);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(100, m_WindowSize_Y - nextWindowSize[1] - 70));
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

		ImVec2 shopWindowSize = ImVec2(500, 400);
		ImGui::SetNextWindowSize(shopWindowSize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetStyle());
		ImGui::Begin("Shop", false, menuFlag);

		if (ImGui::BeginTabBar("ShopTabBar"))
		{
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
					if (ImGui::Button(((*it)->m_type).c_str(), ImVec2(150, 50)))
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

											if (ImGui::Button("Close", ImVec2(80, 0)))
											{
												ImGui::CloseCurrentPopup();
												ShowPopup = []() {};
											}
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
					if (ImGui::Button(((*it)->m_type).c_str(), ImVec2(150, 50)))
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

											if (ImGui::Button("Close", ImVec2(80, 0)))
											{
												ImGui::CloseCurrentPopup();
												ShowPopup = []() {};
											}
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
				}
				
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ShowPopup();

		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void InventoryMenu::Render()
	{
		//New Inventory
		ImVec2 InventoryWindowSize = ImVec2(420, 160);
		ImGui::SetNextWindowSize(InventoryWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - InventoryWindowSize[0] - 80, m_WindowSize_Y - InventoryWindowSize[1] - 20));
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