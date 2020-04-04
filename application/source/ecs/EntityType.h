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

namespace gswy
{
	enum class GameObjectType {
		EMPTY = 0,
		MOUSE,
		BACKGROUND,
		MINIMAP,
		BASE,
		PLAYER,
		ENEMY_PORTAL,
		ENEMY_1,
		ENEMY_2,
		ENEMY_BOSS_1,

		HP_BAR,
		TOWER_BUILD,
		TOWER_FIRE,
		TOWER_ICE,
		TOWER_LIGHTNING,
		FIREBALL,
		FORKED_FIREBALL,
		BOLT_STRIKE,
		BOLT_CHANNEL,
		ICEBALL,
		FORKED_ICEBALL,
		CYCLONE_SFX,
		ENEMY_PROJECTILE,
		RAZOR,

		COIN,

		NUM
	};
}

std::ostream& operator<<(std::ostream& o, gswy::GameObjectType n);
