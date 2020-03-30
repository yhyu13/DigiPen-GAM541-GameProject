/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/29/2020
- End Header ----------------------------*/

#pragma once
#include <iostream>

namespace gswy
{
	enum class GameBuffType {
		EMPTY = 0,
		CHANGE_SPEED_PERCENT,
		CHANGE_SPEED_POINT,
		CHANGE_HP_PERCENT,
		CHANGE_HP_POINT,

		NUM
	};
}

std::ostream& operator<<(std::ostream& o, gswy::GameBuffType n);
