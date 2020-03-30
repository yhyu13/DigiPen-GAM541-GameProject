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

#include "engine-precompiled-header.h"
#include "GameWidgetManager.h"
#include "../inventory-manager/InventoryManager.h"
#include "../skill-manager/SkillManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	}

	void WidgetManager::RenderUI()
	{
		if (m_Hud.GetVisible())              m_Hud.Render();
		if (m_MainMenu.GetVisible())         m_MainMenu.Render();
		if (m_PauseMenu.GetVisible())        m_PauseMenu.Render();
		if (m_ShopMenu.GetVisible())         m_ShopMenu.Render();
		if (m_InventoryMenu.GetVisible())    m_InventoryMenu.Render();
	}

	void MainMenu::Render()
	{
		ImVec2 windowsize = ImVec2(m_WindowSize_X, m_WindowSize_Y);
		ImVec2 nextWindowSize(500, 500);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));

		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		float f = 0.0;
		ImGui::SetWindowFontScale(1.0);

		if (ImGui::Button("New Game", ImVec2(500, 100)))
		{
			manager->InvokeButton("New Game");
		}
		if (ImGui::Button("How To Play", ImVec2(500, 100)))
		{
			manager->InvokeButton("How To Play");
		}
		if (ImGui::Button("Option", ImVec2(500, 100)))
		{
			manager->InvokeButton("Option");
		}
		if (ImGui::Button("Exit", ImVec2(500, 100)))
		{
			manager->InvokeButton("Exit");
		}
		ImGui::Text("This is some useful text.");
		ImGui::End();
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
		m_Mana = 0.5f;
		m_Sanity = 0.5f;
		m_FrameBuffer = FrameBuffer::Create(m_WindowSize_X, m_WindowSize_Y);
	}

	void HUD::Render()
	{	
		//Text : Timer
		ImVec2 nextWindowSize = ImVec2(80, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0], nextWindowSize[1]));
		ImGui::Begin("Timer", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("%i : %i", m_TimerMin, m_TimerSec);
		ImGui::End();
		
		//Text : Wave
		nextWindowSize = ImVec2(80, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 65));
		ImGui::Begin("Wave", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Wave : %i", m_Wave);
		ImGui::End();
		
		//Text : Coin
		nextWindowSize = ImVec2(100, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 135));
		ImGui::Begin("Coin", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Coin : %i", m_Coins);
		ImGui::End();
		
		//Progress Bar : Base Life
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Base Life", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Progress);
		ImGui::End();
		
		//Progress Bar : Mana
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - nextWindowSize[0] - 100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Mana", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Mana);
		ImGui::End();
		
		//Progress Bar : Sanity
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Sanity", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Sanity);
		ImGui::End();
	}

	void PauseMenu::Render()
	{
		ImVec2 windowsize = ImVec2(1920, 1080);
		ImVec2 nextWindowSize(500, 230);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));

		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowFontScale(1.0);
		if (ImGui::Button("Resume", ImVec2(500, 100)))
		{
			manager->InvokeButton("Resume");
		}
		if (ImGui::Button("Main Menu", ImVec2(500, 100)))
		{
			manager->InvokeButton("Main Menu");
		}
		ImGui::Text("This is some useful text.");
		ImGui::End();
	}

	void ShopMenu::Render()
	{
	
		ImVec2 shopWindowSize = ImVec2(500, 400);
		ImGui::SetNextWindowSize(shopWindowSize);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
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
								InventoryManager::GetInstance()->PurchaseActiveItem((*it));
							}
						}
						else
						{
							//Item has been purchased
							unsigned int tabNum = WidgetManager::GetInstance()->GetInventoryMenu().GetCurrentTab();
							if (ImGui::BeginMenu("InstallPopup"))
							{
								if (ImGui::MenuItem("ACTIVE"))
								{
									InventoryManager::GetInstance()->Install(tabNum, 1, (*it));
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
								if (ImGui::MenuItem("SUPPORT 1"))
								{
									unsigned int tabNum = WidgetManager::GetInstance()->GetInventoryMenu().GetCurrentTab();
									InventoryManager::GetInstance()->Install(tabNum, 2, (*it));
								}
								if (ImGui::MenuItem("SUPPORT 2"))
								{
									unsigned int tabNum = WidgetManager::GetInstance()->GetInventoryMenu().GetCurrentTab();
									InventoryManager::GetInstance()->Install(tabNum, 3, (*it));
								}
								if (ImGui::MenuItem("SUPPORT 3"))
								{
									unsigned int tabNum = WidgetManager::GetInstance()->GetInventoryMenu().GetCurrentTab();
									InventoryManager::GetInstance()->Install(tabNum, 4, (*it));
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

		ImGui::End();
	}

	void InventoryMenu::Render()
	{
		//Inventory
		ImVec2 InventoryWindowSize = ImVec2(500, 400);
		ImGui::SetNextWindowSize(InventoryWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - InventoryWindowSize.x, 0));
		ImGui::Begin("Inventory", false, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		if (ImGui::BeginTabBar("InventoryTabBar"))
		{
			if (ImGui::BeginTabItem("1"))
			{
				m_CurrentTab = 1;

				ImGui::Dummy(ImVec2(500, 25));
				//Query : replace name when ACTIVE 1 has been installed
				std::string act1 = SkillManager::GetInstance()->GetSkill(1, 1) ? SkillManager::GetInstance()->GetSkill(1, 1)->m_type.c_str() : "ACTIVE 1";
				ImGui::Button(act1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup1 = SkillManager::GetInstance()->GetSkill(1, 2) ? SkillManager::GetInstance()->GetSkill(1, 2)->m_type.c_str() : "SUPPORT 1";
				ImGui::Button(sup1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup2 = SkillManager::GetInstance()->GetSkill(1, 3) ? SkillManager::GetInstance()->GetSkill(1, 3)->m_type.c_str() : "SUPPORT 2";
				ImGui::Button(sup2.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup3 = SkillManager::GetInstance()->GetSkill(1, 4) ? SkillManager::GetInstance()->GetSkill(1, 4)->m_type.c_str() : "SUPPORT 3";
				ImGui::Button(sup3.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("2"))
			{
				m_CurrentTab = 2;

				ImGui::Dummy(ImVec2(500, 25));
				//Query : replace name when ACTIVE 2 has been installed
				std::string act1 = SkillManager::GetInstance()->GetSkill(2, 1) ? SkillManager::GetInstance()->GetSkill(2, 1)->m_type.c_str() : "ACTIVE 1";
				ImGui::Button(act1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup1 = SkillManager::GetInstance()->GetSkill(2, 2) ? SkillManager::GetInstance()->GetSkill(2, 2)->m_type.c_str() : "SUPPORT 1";
				ImGui::Button(sup1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup2 = SkillManager::GetInstance()->GetSkill(2, 3) ? SkillManager::GetInstance()->GetSkill(2, 3)->m_type.c_str() : "SUPPORT 2";
				ImGui::Button(sup2.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup3 = SkillManager::GetInstance()->GetSkill(2, 4) ? SkillManager::GetInstance()->GetSkill(2, 4)->m_type.c_str() : "SUPPORT 3";
				ImGui::Button(sup3.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("3"))
			{
				m_CurrentTab = 3;

				ImGui::Dummy(ImVec2(500, 25));
				//Query : replace name when ACTIVE 3 has been installed
				std::string act1 = SkillManager::GetInstance()->GetSkill(3, 1) ? SkillManager::GetInstance()->GetSkill(3, 1)->m_type.c_str() : "ACTIVE 1";
				ImGui::Button(act1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup1 = SkillManager::GetInstance()->GetSkill(3, 2) ? SkillManager::GetInstance()->GetSkill(3, 2)->m_type.c_str() : "SUPPORT 1";
				ImGui::Button(sup1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup2 = SkillManager::GetInstance()->GetSkill(3, 3) ? SkillManager::GetInstance()->GetSkill(3, 3)->m_type.c_str() : "SUPPORT 2";
				ImGui::Button(sup2.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup3 = SkillManager::GetInstance()->GetSkill(3, 4) ? SkillManager::GetInstance()->GetSkill(3, 4)->m_type.c_str() : "SUPPORT 3";
				ImGui::Button(sup3.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("4"))
			{
				m_CurrentTab = 4;

				ImGui::Dummy(ImVec2(500, 25));
				//Query : replace name when ACTIVE 4 has been installed
				std::string act1 = SkillManager::GetInstance()->GetSkill(4, 1) ? SkillManager::GetInstance()->GetSkill(4, 1)->m_type.c_str() : "ACTIVE 1";
				ImGui::Button(act1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup1 = SkillManager::GetInstance()->GetSkill(4, 2) ? SkillManager::GetInstance()->GetSkill(4, 2)->m_type.c_str() : "SUPPORT 1";
				ImGui::Button(sup1.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup2 = SkillManager::GetInstance()->GetSkill(4, 3) ? SkillManager::GetInstance()->GetSkill(4, 3)->m_type.c_str() : "SUPPORT 2";
				ImGui::Button(sup2.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				std::string sup3 = SkillManager::GetInstance()->GetSkill(4, 4) ? SkillManager::GetInstance()->GetSkill(4, 4)->m_type.c_str() : "SUPPORT 3";
				ImGui::Button(sup3.c_str(), ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}