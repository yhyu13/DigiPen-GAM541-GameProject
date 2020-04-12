/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/16/2020
- End Header ----------------------------*/

#pragma once
#include <iostream>
#include <vector>

namespace gswy
{
	enum class EventType {
		EMPTY = 0,
		COLLISION,								// Collision event
		SOUND,									// Play sound event
		WEAPON_SOUND,							// Play weapon sound event
		MUTE_SFX,                             // Mute Sound Event
		MUTE_BGM,                             // Mute music Event
		DEATH,									// Death event
		SPAWN,									// Spawning event
		GC,										// Garbage collection event
		FIREWEAPON,								// Use weapon event

		CLICK_ON_TOWER,							// Click on tower event

		ATTACKBASE,								// ?
			
		PLAYER_SET_PENDING_ANIMATION,			// player animation controller event
		PLAYER_READY_TO_CHANGE_ANIMATION,		// player animation controller event

		ADD_BUFF,								// Add buff to entities
		REMOVE_BUFF,							// Remove buff from entities

		FADE,
		_FADE_ING,
		_FADE_END,

		SKILL_USE,
		FORK,

		LOAD_MAIN_MENU,
		LOAD_GAME_WORLD,
		LOAD_TEAM_LOGO,
		LOAD_GAME_LOGO,
		LOAD_CREDIT_SCREEN,
		LOAD_LEVEL_LOGO,
		LOAD_WAVE_CLEAR_LOGO,
		LOAD_LEVEL_CLEAR_LOGO,
		LOAD_WON_LOGO,
		LOAD_LOST_LOGO,
		LOAD_DIED_LOG,
		LOAD_FINAL_WAVE,

		ON_SPLASH_STATE_CHANGE,

		KEY_BIND_EVENT,

		CAN_PLAYER_INPUT,
		ADD_COIN,

		NUM
	};

	extern std::vector<EventType> g_soundTypes;
}

std::ostream& operator<<(std::ostream& o, gswy::EventType n);