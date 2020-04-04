/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/17/2020
- End Header ----------------------------*/

#include "EventType.h"

std::ostream& operator<<(std::ostream& o, gswy::EventType n) {
	const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
	switch (n) {
		PROCESS_VAL(gswy::EventType::EMPTY);
		PROCESS_VAL(gswy::EventType::COLLISION);
		PROCESS_VAL(gswy::EventType::SOUND);
		PROCESS_VAL(gswy::EventType::WEAPON_SOUND);
		PROCESS_VAL(gswy::EventType::DEATH);
		PROCESS_VAL(gswy::EventType::SPAWN);
		PROCESS_VAL(gswy::EventType::GC);
		PROCESS_VAL(gswy::EventType::FIREWEAPON);
		PROCESS_VAL(gswy::EventType::CLICK_ON_TOWER);
		PROCESS_VAL(gswy::EventType::ATTACKBASE);
		PROCESS_VAL(gswy::EventType::PLAYER_SET_PENDING_ANIMATION);
		PROCESS_VAL(gswy::EventType::PLAYER_READY_TO_CHANGE_ANIMATION);
		PROCESS_VAL(gswy::EventType::ADD_BUFF);
		PROCESS_VAL(gswy::EventType::REMOVE_BUFF);
		PROCESS_VAL(gswy::EventType::FADE);
		PROCESS_VAL(gswy::EventType::_FADE_ING);
		PROCESS_VAL(gswy::EventType::_FADE_END);
		PROCESS_VAL(gswy::EventType::SKILL_USE);
		PROCESS_VAL(gswy::EventType::FORK);
		PROCESS_VAL(gswy::EventType::LOAD_MAIN_MENU);
		PROCESS_VAL(gswy::EventType::LOAD_GAME_WORLD);
		PROCESS_VAL(gswy::EventType::LOAD_TEAM_LOGO);
		PROCESS_VAL(gswy::EventType::LOAD_GAME_LOGO);
		PROCESS_VAL(gswy::EventType::LOAD_LEVEL_LOGO);
		PROCESS_VAL(gswy::EventType::LOAD_WAVE_CLEAR_LOGO);
		PROCESS_VAL(gswy::EventType::LOAD_LEVEL_CLEAR_LOGO);
		PROCESS_VAL(gswy::EventType::KEY_BIND_EVENT);
	default: s = "(invalid value)"; break;
	}
#undef PROCESS_VAL
	return o << s;
}
