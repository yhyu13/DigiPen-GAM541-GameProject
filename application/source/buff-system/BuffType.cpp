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
#include "BuffType.h"

std::ostream& operator<<(std::ostream& o, gswy::GameBuffType n) {
	switch (n) {
	case gswy::GameBuffType::EMPTY: return o << "EMPTY";
	case gswy::GameBuffType::CHANGE_SPEED_PERCENT: return o << "CHANGE_SPEED_PERCENT";
	case gswy::GameBuffType::CHANGE_SPEED_POINT: return o << "CHANGE_SPEED_POINT";
	case gswy::GameBuffType::CHANGE_HP_PERCENT: return o << "CHANGE_HP_PERCENT";
	case gswy::GameBuffType::CHANGE_HP_POINT: return o << "CHANGE_HP_POINT";
	default: return o << "(invalid value)";
	}
}
