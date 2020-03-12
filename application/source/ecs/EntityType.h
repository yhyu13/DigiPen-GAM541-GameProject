/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/14/2020
- End Header ----------------------------*/

#pragma once
#include <iostream>
#include "engine/ecs/Entity.h"
namespace gswy
{
	enum class GameObjectType {
		EMPTY = 0,
		BACKGROUND,
		PLAYER,
		ENEMY,
		TOWER_FIRE,
		TOWER_ICE,
		TOWER_LIGHTNING,
		GRENADE,
		FIREBALL,
		BOLT,
		ICEBALL,
		BULLET,

		NUM
	};
}

std::ostream& operator<<(std::ostream& o, gswy::GameObjectType n) {
	switch (n) {
	case gswy::GameObjectType::EMPTY: return o << "EMPTY";
	case gswy::GameObjectType::BACKGROUND: return o << "BACKGROUND";
	case gswy::GameObjectType::PLAYER: return o << "PLAYER";
	case gswy::GameObjectType::ENEMY: return o << "ENEMY";
	case gswy::GameObjectType::TOWER_FIRE: return o << "TOWER_FIRE";
	case gswy::GameObjectType::TOWER_ICE: return o << "TOWER_ICE";
	case gswy::GameObjectType::TOWER_LIGHTNING: return o << "TOWER_LIGHTNING";
	case gswy::GameObjectType::GRENADE: return o << "GRENADE";
	case gswy::GameObjectType::FIREBALL: return o << "FIREBALL";
	case gswy::GameObjectType::BOLT: return o << "BOLT";
	case gswy::GameObjectType::ICEBALL: return o << "ICEBALL";
	case gswy::GameObjectType::BULLET: return o << "BULLET";
	default: return o << "(invalid value)";
	}
}
