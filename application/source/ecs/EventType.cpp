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
	switch (n) {
	case gswy::EventType::EMPTY: return o << "EMPTY";
	case gswy::EventType::COLLISION: return o << "COLLISION";
	case gswy::EventType::SOUND: return o << "SOUND";
	case gswy::EventType::GC: return o << "GC";
	case gswy::EventType::SPAWN: return o << "SPAWN";
	case gswy::EventType::FIREWEAPON: return o << "FIREWEAPON";
	case gswy::EventType::PLAYERWEAPON1: return o << "PLAYERWEAPON1";

	case gswy::EventType::DEATH: return o << "DEATH";

	case gswy::EventType::FADE: return o << "FADEIN";
	case gswy::EventType::_FADE_ING: return o << "_FADEIN_ING";
	case gswy::EventType::_FADE_END: return o << "_FADEIN_END";

	case gswy::EventType::SKILL_ADDITION: return o << "ADD SKILL";
	case gswy::EventType::SKILL_REMOVAL: return o << "REMOVE SKILL";
	case gswy::EventType::SKILL_ACTIVATION: return o << "ACTIVATE SKILL";
	case gswy::EventType::SKILL_UPGRADE: return o << "UPGRADE SKILL";
	case gswy::EventType::SKILL_USE: return o << "USE SKILL";

	default: return o << "(invalid value)";
	}
}
