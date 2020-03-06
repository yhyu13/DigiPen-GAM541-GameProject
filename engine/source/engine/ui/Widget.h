/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Widget.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/26/2020
- End Header ----------------------------*/
#pragma once
#include "imgui.h"
 
namespace gswy {
	
	class Widget
	{
	public:
		virtual ~Widget() {}
		virtual void Render() = 0;
		inline void SetVisible(bool vis) { IsVisible = vis; }
		inline bool GetVisible() const { return IsVisible; }
	protected:
		bool IsVisible = true;
		unsigned int m_WindowSize_X = 1920;
		unsigned int m_WindowSize_Y = 1080;
	};

	//Player HP, player skill cooldown, coins that build towers, #of waves
	class HUD : public Widget
	{
	public:
		void Render() override;
	};

	//new game, continue, option, how to play, quit
	class MainMenu : public Widget
	{
	public:
		void Render() override;
	};

	//Resume, option, how to play, quit & save
	class PauseMenu : public Widget
	{
	public:
		void Render() override;
	};

	//Purchase tower upgrades, purchase player skill upgrades
	class ShopMenu : public Widget
	{
	public:
		void Render() override;
	};

	//Allocate attributes, install support skills into basic skills, account basic stats (#HP, #monster killed, #experience)
	class InventoryMenu : public Widget
	{
	public:
		void Render() override;
	};
}