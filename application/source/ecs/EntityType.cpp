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
	const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
	switch (n) {
		PROCESS_VAL(gswy::GameObjectType::EMPTY);
		PROCESS_VAL(gswy::GameObjectType::MOUSE);
		PROCESS_VAL(gswy::GameObjectType::BACKGROUND);
		PROCESS_VAL(gswy::GameObjectType::MINIMAP);
		PROCESS_VAL(gswy::GameObjectType::BASE);
		PROCESS_VAL(gswy::GameObjectType::PLAYER);
		PROCESS_VAL(gswy::GameObjectType::ENEMY_PORTAL);
		PROCESS_VAL(gswy::GameObjectType::ENEMY_1);
		PROCESS_VAL(gswy::GameObjectType::ENEMY_2);
		PROCESS_VAL(gswy::GameObjectType::ENEMY_BOSS_1);
		PROCESS_VAL(gswy::GameObjectType::HP_BAR);
		PROCESS_VAL(gswy::GameObjectType::TOWER_BUILD);
		PROCESS_VAL(gswy::GameObjectType::TOWER_FIRE);
		PROCESS_VAL(gswy::GameObjectType::TOWER_ICE);
		PROCESS_VAL(gswy::GameObjectType::TOWER_LIGHTNING);
		PROCESS_VAL(gswy::GameObjectType::FIREBALL);
		PROCESS_VAL(gswy::GameObjectType::FORKED_FIREBALL);
		PROCESS_VAL(gswy::GameObjectType::BOLT_STRIKE);
		PROCESS_VAL(gswy::GameObjectType::BOLT_CHANNEL);
		PROCESS_VAL(gswy::GameObjectType::ICEBALL);
		PROCESS_VAL(gswy::GameObjectType::FORKED_ICEBALL);
		PROCESS_VAL(gswy::GameObjectType::CYCLONE_SFX);
		PROCESS_VAL(gswy::GameObjectType::ENEMY_PROJECTILE);
		PROCESS_VAL(gswy::GameObjectType::RAZOR);
		PROCESS_VAL(gswy::GameObjectType::TEAM_LOGO);
		PROCESS_VAL(gswy::GameObjectType::GAME_LOGO);
		PROCESS_VAL(gswy::GameObjectType::CREDITS);
	default: s = "(invalid value)"; break;
	}
#undef PROCESS_VAL
	return o << s;
}