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
#include "engine/events/Event.h"
namespace gswy
{
	enum class EventType {
		EMPTY = 0,
		COLLISION,
		SOUND,
		DEATH,

		PLAYERWEAPON1,
		PLAYERWEAPON2,
		PLAYERWEAPON3,
		PLAYERSKILL1,
		PLAYERSKILL2,
		PLAYERSKILL3,
		NUM
	};
}

std::ostream& operator<<(std::ostream& o, gswy::EventType n) {
	switch (n) {
	case gswy::EventType::EMPTY: return o << "EMPTY";
	case gswy::EventType::COLLISION: return o << "COLLISION";
	case gswy::EventType::PLAYERWEAPON1: return o << "PLAYERWEAPON1";
	default: return o << "(invalid value)";
	}
}
