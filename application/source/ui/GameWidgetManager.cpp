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
			std::cout << "New game Click!" << std::endl;
		}
		if (ImGui::Button("Continue", ImVec2(500, 100)))
		{
			std::cout << "Continue Click!" << std::endl;
		}
		if (ImGui::Button("Option", ImVec2(500, 100)))
		{
			std::cout << "Option Click!" << std::endl;
		}
		if (ImGui::Button("Quit", ImVec2(500, 100)))
		{
			std::cout << "Quit Click!" << std::endl;
		}
		ImGui::Text("This is some useful text.");
		ImGui::End();
	}

	void HUD::Render()
	{
		//Text : Wave
		ImVec2 nextWindowSize = ImVec2(60, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X / 2 - nextWindowSize[0], m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Wave", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Wave 0");
		ImGui::End();

		//Progress Bar : Mana
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - nextWindowSize[0] - 100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Mana", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(0.5f);
		ImGui::End();

		//Progress Bar : Sanity
		nextWindowSize = ImVec2(500, 20);
		ImGui::SetNextWindowSize(nextWindowSize);
		ImGui::SetNextWindowPos(ImVec2(100, m_WindowSize_Y - nextWindowSize[1] - 100));
		ImGui::Begin("Sanity", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::ProgressBar(0.5f);
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
			std::cout << "Resume Click!" << std::endl;
		}
		if (ImGui::Button("How To Play", ImVec2(500, 100)))
		{
			std::cout << "How To Play Click!" << std::endl;
		}
		if (ImGui::Button("Option", ImVec2(500, 100)))
		{
			std::cout << "Option Click!" << std::endl;
		}
		if (ImGui::Button("Quit & Save", ImVec2(500, 100)))
		{
			std::cout << "Quit & Save Click!" << std::endl;
		}
		ImGui::Text("This is some useful text.");
		ImGui::End();
	}

	void ShopMenu::Render()
	{
		ImVec2 shopWindowSize = ImVec2(500, 600);
		ImGui::SetNextWindowSize(shopWindowSize);
		ImGui::SetNextWindowPos(ImVec2(m_WindowSize_X - shopWindowSize.x, 0));
		ImGui::Begin("Shop", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		if (ImGui::BeginTabBar("ShopTabBar"))
		{
			if (ImGui::BeginTabItem("Totem"))
			{
				ImGui::Button("Skill", ImVec2(100, 100));
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Acitve Skill"))
			{
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
			if (ImGui::BeginTabItem("Support Skill"))
			{
				ImGui::Button("Skill", ImVec2(100, 100)); ImGui::SameLine();
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
		ImGui::SetNextWindowSize(ImVec2(500, 600));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Inventory", false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		if (ImGui::BeginTabBar("InventoryTabBar"))
		{
			if (ImGui::BeginTabItem("Skill 1"))
			{
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Skill 2"))
			{
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Devotion Skill"))
			{
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100)); ImGui::SameLine();
				ImGui::Button("Skill Slot", ImVec2(100, 100));
				ImGui::Separator();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}