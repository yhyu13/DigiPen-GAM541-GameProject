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

/*
Return the lookat angle in radian
*/
float gswy::LookAt(const vec2& delta)
{
	return (delta.y) > 0 ? -atanf(delta.x / (delta.y + 1e-4f)) : PI - atanf(delta.x / (delta.y + 1e-4f));
}

vec2 gswy::RadToVec(float rotation)
{
	return vec2(cosf(rotation), sinf(rotation));
}

vec2 gswy::ToVec(float rotation)
{
	return vec2(-sinf(rotation),cosf(rotation));
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& n) {
	o << '(' << n.x << ',' << n.y << ',' << n.z << ") ";
	return o;
}

std::ostream& operator<<(std::ostream& o, const glm::vec2& n) {
	o << '(' << n.x << ',' << n.y << ") ";
	return o;
}