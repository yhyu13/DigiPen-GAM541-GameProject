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
#include "engine/renderer/OrthographicCameraController.h"

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
		void  SetBaseHP(float val) { m_BaseHP = val; }
		void  SetPlayerHP(float val) { m_PlayerHP = val; }
		void  SetGameProgress(float val) { m_Progress = val; }

		std::shared_ptr<Texture2D> m_Texture_Coin;

		ImVec2 m_WaveWindowSize;
		ImVec2 m_GameProgressWindowSize;
		ImVec2 m_TimeWindowSize;
		ImVec2 m_CoinWindowSize;
		ImVec2 m_PlayerHPWindowSize;
		ImVec2 m_BaseHPWindowSize;

	private:
		int m_TimerMin;
		int m_TimerSec;
		int m_Wave;
		int m_Level;
		int m_Coins;
		float m_Progress;
		float m_BaseHP;
		float m_PlayerHP;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;
	};

	//new game, continue, option, how to play, quit
	class MainMenu : public Widget
	{
	public:
		MainMenu() { 
			IsVisible = false; 
			m_MainMenu = { 500, 535 };
		}
		void Init() override
		{
			m_Texture_NewGame = Texture2D::Create(m_TexturePath_NewGame);
			m_Texture_HowToPlay = Texture2D::Create(m_TexturePath_HowToPlay);
			m_Texture_Option = Texture2D::Create(m_TexturePath_Option);
			m_Texture_Credits = Texture2D::Create(m_TexturePath_Credits);
			m_Texture_QuitGame = Texture2D::Create(m_TexturePath_QuitGame);
		}
		void Render() override;
		WidgetManager* manager;

		std::string m_TexturePath_NewGame;
		std::string m_TexturePath_HowToPlay;
		std::string m_TexturePath_Option;
		std::string m_TexturePath_Credits;
		std::string m_TexturePath_QuitGame;

		std::shared_ptr<Texture2D> m_Texture_NewGame;
		std::shared_ptr<Texture2D> m_Texture_HowToPlay;
		std::shared_ptr<Texture2D> m_Texture_Option;
		std::shared_ptr<Texture2D> m_Texture_Credits;
		std::shared_ptr<Texture2D> m_Texture_QuitGame;

		ImVec2 m_MainMenu;
	};

	//Resume, option, how to play, quit & save
	class PauseMenu : public Widget
	{
	public:
		PauseMenu() { IsVisible = false; };
		void Init() override 
		{
			m_Texture_ResumeGame = Texture2D::Create(m_TexturePath_ResumeGame);
			m_Texture_MainMenu = Texture2D::Create(m_TexturePath_MainMenu);
			m_PauseMenuWindowSize = { 500, 640 };
		}
		void Render() override;
		WidgetManager* manager;

		std::string m_TexturePath_ResumeGame;
		std::string m_TexturePath_MainMenu;

		ImVec2 m_PauseMenuWindowSize;
	private:
		std::shared_ptr<Texture2D> m_Texture_ResumeGame;
		std::shared_ptr<Texture2D> m_Texture_MainMenu;
	};

	//Purchase tower upgrades, purchase player skill upgrades
	class ShopMenu : public Widget
	{
	public:
		ShopMenu() { 
			IsVisible = false; 
			m_ShopMenuWindowSize = { 420, 400 };
		};
		void Render() override;
		WidgetManager* manager;

		ImVec2 m_ShopMenuWindowSize;

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
			m_InventoryWindowSize = { 420, 160 };
		};
		void Render() override;
		WidgetManager* manager;

		std::shared_ptr<Texture2D> m_Texture_Acitve1;
		std::shared_ptr<Texture2D> m_Texture_Acitve2;
		std::shared_ptr<Texture2D> m_Texture_Acitve3;
		std::shared_ptr<Texture2D> m_Texture_Acitve4;

		std::shared_ptr<Texture2D> m_Texture_Support1_2;
		std::shared_ptr<Texture2D> m_Texture_Support1_3;
		std::shared_ptr<Texture2D> m_Texture_Support1_4;
		std::shared_ptr<Texture2D> m_Texture_Support2_2;
		std::shared_ptr<Texture2D> m_Texture_Support2_3;
		std::shared_ptr<Texture2D> m_Texture_Support2_4;
		std::shared_ptr<Texture2D> m_Texture_Support3_2;
		std::shared_ptr<Texture2D> m_Texture_Support3_3;
		std::shared_ptr<Texture2D> m_Texture_Support3_4;
		std::shared_ptr<Texture2D> m_Texture_Support4_2;
		std::shared_ptr<Texture2D> m_Texture_Support4_3;
		std::shared_ptr<Texture2D> m_Texture_Support4_4;

		ImVec2 m_InventoryWindowSize;
	};

	class OptionMenu : public Widget
	{
	public:
		OptionMenu() {
			IsVisible = false;  
		}
		void Init() override {}
		void Render() override;
		void Update() override;

		void SetCallFromMainMenu(bool b) { m_CallFromMainMenu = b; }

		bool m_MuteMusic = false;
		bool m_MuteSFX = false;
		bool m_CallFromMainMenu = false;
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
		
		void PushPopModalStyle();
		void PopPopupModalStyle();
		void PushTabStyle();
		void PopTabStyle();

		HUD& GetHUD() { return m_Hud; }
		MainMenu& GetMainMenu() { return m_MainMenu; }
		PauseMenu& GetPauseMenu() { return m_PauseMenu; }
		OptionMenu& GetOptionMenu() { return m_OptionMenu; }
		ShopMenu& GetShopMenu() { return m_ShopMenu; }
		InventoryMenu& GetInventoryMenu() { return m_InventoryMenu; }

	private:
		WidgetManager();
		static WidgetManager* s_instance;
		ButtonInvokeFunction buttonInvoker;

		HUD m_Hud;
		MainMenu m_MainMenu;
		PauseMenu m_PauseMenu;
		OptionMenu m_OptionMenu;
		ShopMenu m_ShopMenu;
		InventoryMenu m_InventoryMenu;
	};
}

