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

//#include "engine-precompiled-header.h"
#include "GameWidgetManager.h"
#include "inventory-manager/InventoryManager.h"
#include "tilemap/GameLevelMapManager.h"
#include "skill-manager/SkillManager.h"
#include "engine/audio/AudioManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json/json.h>

namespace gswy {

	WidgetManager* WidgetManager::s_instance = 0;

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
		if (m_MainMenu.GetVisible())         m_MainMenu.Render();
		if (m_PauseMenu.GetVisible())        m_PauseMenu.Render();
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
			GetMainMenu().m_TexturePath_Exit = widgetData["Exit"].asString();
			GetPauseMenu().m_TexturePath_Resume = widgetData["Resume"].asString();
			GetPauseMenu().m_TexturePath_MainMenu = widgetData["MainMenu"].asString();
		}
	}

	void MainMenu::Render()
	{
		ImVec2 windowsize = ImVec2(m_WindowSize_X, m_WindowSize_Y);
		ImVec2 nextWindowSize(500, 428);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255,137,20));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
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
			manager->InvokeButton("Option");
		}
		if (ImGui::ImageButton((void*)m_Texture_Exit->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Exit");
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
	}

	void PauseMenu::Render()
	{
		ImVec2 windowsize = ImVec2(1920, 1080);
		ImVec2 nextWindowSize(500, 230);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 5));
		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowFontScale(1.0);
		if (ImGui::ImageButton((void*)m_Texture_Resume->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Resume");
		}
		if (ImGui::ImageButton((void*)m_Texture_MainMenu->GetRendererID(), ImVec2(480, 100), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0, 0, 0, 1)))
		{
			AudioManager::GetInstance()->PlaySound("click_sound");
			manager->InvokeButton("Main Menu");
		}
		ImGui::End();
		ImGui::PopStyleColor(1);
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
	}

	void HUD::Render()
	{	
		//Text : Timer
		ImVec2 nextWindowSize = ImVec2(80, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, 55));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Timer", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("%i : %i", m_TimerMin, m_TimerSec);
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		
		//Text : Wave
		nextWindowSize = ImVec2(80, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Wave", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Wave : %i", m_Wave);
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		
		//Text : Coin
		nextWindowSize = ImVec2(140, 140);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(0, m_WindowSize_Y - nextWindowSize[1] - 20));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Coin", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::Text("Coin : %i", m_Coins);
		ImGui::Image(0, ImVec2(130, 130));
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);

		//Progress Bar : Base HP
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, 20));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Base HP", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::AlignFirstTextHeightToWidgets();
		ImGui::Text("Base HP");
		ImGui::SameLine();
		ImGui::ProgressBarCustomColor(m_BaseHP, ImVec4((1.0f - m_BaseHP), m_BaseHP, 0, 1));
		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(2);
		
		//Progress Bar : Player HP
		nextWindowSize = ImVec2(250, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(600, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
		ImGui::Begin("Player HP", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
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
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(255, 137, 20));
		ImGui::Begin("Shop", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

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

										if (ImGui::BeginPopupModal("popup"))
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
				auto supportItems = InventoryManager::GetInstance()->GetSupportItems();
				auto sbegin = supportItems.begin();
				auto send = supportItems.end();
				for (auto it = sbegin; it != send; ++it)
				{
					bool bSupportPurchased = (*it)->m_purchased;
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
							if (ImGui::Selectable("SupportPurchase"))
							{
								InventoryManager::GetInstance()->PurchaseSupportItem((*it));
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

					ImGui::SameLine();

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
	}

	void InventoryMenu::Render()
	{
		//New Inventory
		ImVec2 InventoryWindowSize = ImVec2(400, 140);
		ImGui::SetNextWindowSize(InventoryWindowSize);
		ImGui::SetNextWindowPos(ImVec2(150, m_WindowSize_Y - InventoryWindowSize[1] - 20));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
		ImGui::PushStyleColor(ImGuiCol_Button, { 90 ,90 ,90 ,255 });
		ImGui::Begin("Inventory", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		{
			if (SkillManager::GetInstance()->GetSkill(1, 1))
			{
				m_Texture_Acitve1 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(1, 1)->m_icon);
				ImGui::ImageButton((void*)m_Texture_Acitve1->GetRendererID(), ImVec2(94, 94));
			}
			else
			{
				ImGui::Button("ACTIVE 1", ImVec2(94, 94));
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(2, 1))
			{
				m_Texture_Acitve2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(2, 1)->m_icon);
				ImGui::ImageButton((void*)m_Texture_Acitve2->GetRendererID(), ImVec2(94, 94));
			}
			else
			{
				ImGui::Button("ACTIVE 2", ImVec2(94, 94));				
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(3, 1))
			{
				m_Texture_Acitve2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(3, 1)->m_icon);
				ImGui::ImageButton((void*)m_Texture_Acitve2->GetRendererID(), ImVec2(94, 94));
			}
			else
			{
				ImGui::Button("ACTIVE 3", ImVec2(94, 94));
			}
		}
		ImGui::SameLine();
		{
			if (SkillManager::GetInstance()->GetSkill(4, 1))
			{
				m_Texture_Acitve2 = Texture2D::Create(SkillManager::GetInstance()->GetSkill(4, 1)->m_icon);
				ImGui::ImageButton((void*)m_Texture_Acitve2->GetRendererID(), ImVec2(94, 94));
			}
			else
			{
				ImGui::Button("ACTIVE 4", ImVec2(94, 94));
			}
		}
		{
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
			ImGui::SameLine();
			ImGui::Image(0, ImVec2(30, 30));
		}
		ImGui::End();
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(1);
	}
}