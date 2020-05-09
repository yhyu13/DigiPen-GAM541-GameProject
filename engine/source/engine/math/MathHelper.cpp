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

#include "engine-precompiled-header.h"
#include "MathHelper.h"

namespace gswy
{
	MathHelper* MathHelper::instance = nullptr;
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& n) {
	o << '(' << n.x << ',' << n.y << ',' << n.z << ") ";
	return o;
}

std::ostream& operator<<(std::ostream& o, const glm::vec2& n) {
	o << '(' << n.x << ',' << n.y << ") ";
	return o;
}