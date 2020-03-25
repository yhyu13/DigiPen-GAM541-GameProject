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

#include "EntityType.h"

std::ostream& operator<<(std::ostream& o, gswy::GameObjectType n) {
	switch (n) {
	case gswy::GameObjectType::EMPTY: return o << "EMPTY";
	case gswy::GameObjectType::MOUSE: return o << "MOUSE";
	case gswy::GameObjectType::BACKGROUND: return o << "BACKGROUND";
	case gswy::GameObjectType::MINIMAP: return o << "MINIMAP";
	case gswy::GameObjectType::BASE: return o << "BASE";
	case gswy::GameObjectType::PLAYER: return o << "PLAYER";
	case gswy::GameObjectType::ENEMY_PORTAL: return o << "ENEMY_PORTAL";
	case gswy::GameObjectType::ENEMY_1: return o << "ENEMY_1";
	case gswy::GameObjectType::ENEMY_2: return o << "ENEMY_2";
	case gswy::GameObjectType::ENEMY_BOSS_1: return o << "ENEMY_BOSS_1";

	case gswy::GameObjectType::HP_BAR: return o << "HP_BAR";
	case gswy::GameObjectType::TOWER_BUILD: return o << "TOWER_BUILD";
	case gswy::GameObjectType::TOWER_FIRE: return o << "TOWER_FIRE";
	case gswy::GameObjectType::TOWER_ICE: return o << "TOWER_ICE";
	case gswy::GameObjectType::TOWER_LIGHTNING: return o << "TOWER_LIGHTNING";
	case gswy::GameObjectType::FIREBALL: return o << "FIREBALL";
	case gswy::GameObjectType::BOLT: return o << "BOLT";
	case gswy::GameObjectType::ICEBALL: return o << "ICEBALL";
	case gswy::GameObjectType::ENEMY_PROJECTILE: return o << "ENEMY_PROJECTILE";
	default: return o << "(invalid value)";
	}
}