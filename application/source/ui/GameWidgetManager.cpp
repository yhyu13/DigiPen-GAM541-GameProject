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

		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
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
		IsVisible = true; 
		m_TimerMin = 0; 
		m_TimerSec = 0; 
		m_Wave = 0; 
		m_Coins = 0;
		m_Progress = 0.5f;
		m_Mana = 0.5f;
		m_Sanity = 0.5f;
		m_FrameBuffer = FrameBuffer::Create(m_WindowSize_X, m_WindowSize_Y);
	}

	void HUD::Render()
	{	
		//Text : Timer
		ImVec2 nextWindowSize = ImVec2(60, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0], nextWindowSize[1]));
		ImGui::Begin("Timer", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("%i : %i", m_TimerMin, m_TimerSec);
		ImGui::End();
		
		//Text : Wave
		nextWindowSize = ImVec2(60, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 65));
		ImGui::Begin("Wave", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Wave : %i", m_Wave);
		ImGui::End();
		
		//Text : Coin
		nextWindowSize = ImVec2(60, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 135));
		ImGui::Begin("Coin", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Coin : %i", m_Coins);
		ImGui::End();
		
		//Progress Bar : Base Life
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0] / 2, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Base Life", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Progress);
		ImGui::End();
		
		//Progress Bar : Mana
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - nextWindowSize[0] - 100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Mana", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Mana);
		ImGui::End();
		
		//Progress Bar : Sanity
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Sanity", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(m_Sanity);
		ImGui::End();
	}

	void PauseMenu::Render()
	{
		ImVec2 windowsize = ImVec2(1920, 1080);
		ImVec2 nextWindowSize(500, 500);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(windowsize[0] / 2 - nextWindowSize[0] / 2, windowsize[1] / 2 - nextWindowSize[1] / 2));

		ImGui::Begin("A new world", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		float f = 0.0;
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
		ImGui::Begin("Shop", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		if (ImGui::BeginTabBar("ShopTabBar"))
		{
			if (ImGui::BeginTabItem("ACTIVE"))
			{
				ImGui::Dummy(ImVec2(250, 15));
				ImGui::Button("FIRE 1", ImVec2(50, 25));
				ImGui::Dummy(ImVec2(250, 15));
				ImGui::Button("FIRE 2", ImVec2(50, 25));
				ImGui::Dummy(ImVec2(250, 15));
				ImGui::Button("ICE 1", ImVec2(50, 25));
				ImGui::Dummy(ImVec2(250, 15));
				ImGui::Button("ICE 2", ImVec2(50, 25));
				ImGui::Dummy(ImVec2(250, 15));

				ImGui::SetCursorPos(ImVec2(50, shopWindowSize.y - 50));
				ImGui::Button("PURCHASE", ImVec2(150, 25));
				ImGui::SetCursorPos(ImVec2(shopWindowSize.x - 200, shopWindowSize.y - 50));
				ImGui::Button("INSTALL", ImVec2(150, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("SUPPORT"))
			{
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill", ImVec2(100, 100));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Passive Skill"))
			{
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill", ImVec2(100, 100));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Devotion Skill"))
			{
				ImGui::Button("Skill", ImVec2(100, 100));
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
		ImGui::Begin("Inventory", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		if (ImGui::BeginTabBar("InventoryTabBar"))
		{
			if (ImGui::BeginTabItem("1"))
			{
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("ACTIVE 1", ImVec2(300, 50));	
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 1", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 2", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 3", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("2"))
			{
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("ACTIVE 1", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 1", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 2", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 3", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("3"))
			{
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("ACTIVE 1", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 1", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 2", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::Button("SUPPORT 3", ImVec2(300, 50));
				ImGui::Dummy(ImVec2(500, 25));
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}