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
#include <glm/glm.hpp>
using namespace glm;

#ifndef PI
#define PI 3.14159265359
#endif // !PI

#ifndef PI2
#define PI2 6.28318530718
#endif // !PI

#ifndef PI_INVERSE
#define PI_INVERSE 0.31830988618
#endif // !PI

#ifndef PI2_INVERSE
#define PI2_INVERSE 0.15915494309
#endif // !PI

#ifndef DEG2RAD
#define DEG2RAD 0.01745329251
#endif // !PI

#ifndef RAD2DEG
#define RAD2DEG 57.2957795131
#endif // !PI

namespace gswy
{
	/*
		Return the lookat angle in radian
	*/
	float LookAt(const vec2& delta);
}