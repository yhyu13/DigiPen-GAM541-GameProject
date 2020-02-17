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

namespace gswy
{
	enum GameObjectType {
		EMPTY = 0,
		BACKGROUND,
		PLAYER,
		ENEMY,

		NUM
	};
}

ostream& operator<<(ostream& o, GameObjectType n) {
	switch (n) {
	case EMPTY: return o << "EMPTY";
	case BACKGROUND: return o << "BACKGROUND";
	case PLAYER: return o << "PLAYER";
	case ENEMY: return o << "ENEMY";
	default: return o << "(invalid value)";
	}
}
