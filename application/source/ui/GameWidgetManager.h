/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: GameWidgetManager.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 03/12/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ui/Widget.h"
#include "engine/renderer/Buffer.h"

namespace gswy {

	class WidgetManager;

	//Player HP, player skill cooldown, coins that build towers, #of waves
	class HUD : public Widget
	{
	public:
		HUD();
		void Render() override;

		std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return m_FrameBuffer; }
		WidgetManager* manager;

	private:
		int m_TimerMin;
		int m_TimerSec;
		int m_Wave;
		int m_Coins;
		float m_Progress;
		float m_Mana;
		float m_Sanity;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;
		
	};

	//new game, continue, option, how to play, quit
	class MainMenu : public Widget
	{
	public:
		MainMenu() { IsVisible = false; };
		void Render() override;
		WidgetManager* manager;
	};

	//Resume, option, how to play, quit & save
	class PauseMenu : public Widget
	{
	public:
		PauseMenu() { IsVisible = false; };
		void Render() override;
		WidgetManager* manager;
	};

	//Purchase tower upgrades, purchase player skill upgrades
	class ShopMenu : public Widget
	{
	public:
		ShopMenu() { IsVisible = false; };
		void Render() override;
		WidgetManager* manager;
	};

	//Allocate attributes, install support skills into basic skills, account basic stats (#HP, #monster killed, #experience)
	class InventoryMenu : public Widget
	{
	public:
		InventoryMenu() { IsVisible = false; };
		void Render() override;
		WidgetManager* manager;
	};

	class WidgetManager
	{
	public:
		typedef std::function<void(const std::string&)> ButtonInvokeFunction;

		static WidgetManager* GetInstance();

		void RenderUI();
		void SetVisible(Widget& widget, bool visible) { widget.SetVisible(visible); }
		bool GetVisible(Widget& widget) { return widget.GetVisible(); }

		void SetButtonInvoker(const ButtonInvokeFunction& f) { buttonInvoker = f; }
		void InvokeButton(const std::string& name) { buttonInvoker(name); }

		HUD& GetHUD() { return m_Hud; }
		MainMenu& GetMainMenu() { return m_MainMenu; }
		PauseMenu& GetPauseMenu() { return m_PauseMenu; }
		ShopMenu& GetShopMenu() { return m_ShopMenu; }
		InventoryMenu& GetInventoryMenu() { return m_InventoryMenu; }

	private:
		WidgetManager();
		static WidgetManager* s_instance;
		ButtonInvokeFunction buttonInvoker;

		HUD m_Hud;
		MainMenu m_MainMenu;
		PauseMenu m_PauseMenu;
		ShopMenu m_ShopMenu;
		InventoryMenu m_InventoryMenu;
	};
}

