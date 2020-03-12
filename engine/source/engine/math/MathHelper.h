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
//#define GLM_FORCE_SSE2
#define GLM_FORCE_AVX2
#define GLM_FORCE_INLINE
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

#define RAND_I(LO, HI) LO + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (HI - LO)))
#define RAND_F(LO, HI) LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)))
#define IN_RANGE(a, x, y) (a >= x && a <= y)

#define MAX(x, y) (x>y)?x:y
#define MIN(x, y) (x<y)?x:y

/*
	Z_ORDER(0) = 0.00,Z_ORDER(1) = 0.001,Z_ORDER(3) = 0.003,
*/
#define Z_ORDER(x) static_cast<float>(x)*0.0001f

namespace gswy
{
	/*
		Return the lookat angle in radian
	*/
	float LookAt(const vec2& delta);

	/*
	Return the vec2 form of a angle used in game (rotated 90 degree counterclockwise)
	*/
	vec2 ToVec(float rotation);

	/*
	Return the vec2 form of a angle in radiance (usual coordinate)
	*/
	vec2 RadToVec(float rotation);
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& n);
std::ostream& operator<<(std::ostream& o, const glm::vec2& n);