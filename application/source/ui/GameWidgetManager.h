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
#include "inventory-manager/Item.h"
#include "engine/renderer/Texture.h"

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

		void  SetTimeMinute(int timeMin) { m_TimerMin = timeMin; }
		void  SetTimeSecond(int timeSec) { m_TimerSec = timeSec; }
		void  SetWave(int wave) { m_Wave = wave; }
		void  SetCoinNum(int coinNum) { m_Coins = coinNum; }
		void  SetLevel(float lvl) { m_Level = lvl; }

	private:
		int m_TimerMin;
		int m_TimerSec;
		int m_Wave;
		int m_Level;
		int m_Coins;
		float m_Progress;
		float m_Mana;
		float m_PlayerHealth;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;
	};

	//new game, continue, option, how to play, quit
	class MainMenu : public Widget
	{
	public:
		MainMenu() { IsVisible = false; }
		void Init() override
		{
			m_Texture_NewGame = Texture2D::Create(m_TexturePath_NewGame);
			m_Texture_HowToPlay = Texture2D::Create(m_TexturePath_HowToPlay);
			m_Texture_Option = Texture2D::Create(m_TexturePath_Option);
			m_Texture_Exit = Texture2D::Create(m_TexturePath_Exit);
		}
		void Render() override;
		WidgetManager* manager;

		std::string m_TexturePath_NewGame;
		std::string m_TexturePath_HowToPlay;
		std::string m_TexturePath_Option;
		std::string m_TexturePath_Exit;
	private:
		std::shared_ptr<Texture2D> m_Texture_NewGame;
		std::shared_ptr<Texture2D> m_Texture_HowToPlay;
		std::shared_ptr<Texture2D> m_Texture_Option;
		std::shared_ptr<Texture2D> m_Texture_Exit;
	};

	//Resume, option, how to play, quit & save
	class PauseMenu : public Widget
	{
	public:
		PauseMenu() { IsVisible = false; };
		void Init() override 
		{
			m_Texture_Resume = Texture2D::Create(m_TexturePath_Resume);
			m_Texture_MainMenu = Texture2D::Create(m_TexturePath_MainMenu);
		}
		void Render() override;
		WidgetManager* manager;

		std::string m_TexturePath_Resume;
		std::string m_TexturePath_MainMenu;
	private:
		std::shared_ptr<Texture2D> m_Texture_Resume;
		std::shared_ptr<Texture2D> m_Texture_MainMenu;
	};

	//Purchase tower upgrades, purchase player skill upgrades
	class ShopMenu : public Widget
	{
	public:
		ShopMenu() { IsVisible = false; };
		void Render() override;
		WidgetManager* manager;

	private:
		std::pair<std::shared_ptr<Item>, bool> m_ClickedItem;
		std::pair<std::shared_ptr<Item>, bool> m_ClickedSupportItem;
		bool m_OpenSupportWindow = false;
	};

	//Allocate attributes, install support skills into basic skills, account basic stats (#HP, #monster killed, #experience)
	class InventoryMenu : public Widget
	{
	public:
		InventoryMenu() {
			IsVisible = false; 
			m_CurrentTab = 1; 
		};
		void Render() override;
		WidgetManager* manager;
		unsigned int GetCurrentTab() const { return m_CurrentTab; }
	private:
		unsigned int m_CurrentTab;
	};

	class WidgetManager
	{
	public:
		typedef std::function<void(const std::string&)> ButtonInvokeFunction;

		static WidgetManager* GetInstance();

		void LoadWidget(const std::string& filepath);
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

