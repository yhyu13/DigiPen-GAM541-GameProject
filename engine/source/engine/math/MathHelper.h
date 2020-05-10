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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp >
#include "engine/exception/EngineException.h"

using namespace glm;

#ifndef PI
#define PI 3.14159265359f
#endif // !PI

#ifndef PI2
#define PI2 6.28318530718f
#endif // !PI

#ifndef PI_INVERSE
#define PI_INVERSE 0.31830988618f
#endif // !PI

#ifndef PI2_INVERSE
#define PI2_INVERSE 0.15915494309f
#endif // !PI

#ifndef DEG2RAD
#define DEG2RAD 0.01745329251f
#endif // !PI

#ifndef RAD2DEG
#define RAD2DEG 57.2957795131f
#endif // !PI
//
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
	typedef float degree;
	typedef float rad;
	typedef glm::ivec3 Vec3i;
	typedef glm::vec3 Vec3f;
	typedef glm::quat Quaternion;
	typedef glm::vec3 EulerAnglesDegree;
	typedef glm::vec3 EulerAnglesRad;
	typedef glm::vec3 RotationAxis;
	typedef glm::mat3 Mat3;
	typedef glm::mat4 Mat4;

	class MathHelper
	{
	public:
		static MathHelper* instance;
	public:
		/*
			Return the lookat angle in radian
		*/
		virtual float LookAt(const glm::vec2& delta)
		{
			throw NotImplementedException();
		}

		/*
		Return the vec2 form of a angle used in game
		*/
		virtual glm::vec2 ToVec(float rotation)
		{
			throw NotImplementedException();
		}

		virtual Mat3 ToMat3(Quaternion quat)
		{
			return glm::toMat3(quat);
		}

		virtual Mat4 ToMat4(Quaternion quat)
		{
			return glm::toMat4(quat);
		}

		virtual Quaternion ToQuaternionMat3(Mat3 mat)
		{
			return glm::toQuat(mat);
		}

		virtual Quaternion ToQuaternionMat4(Mat4 mat)
		{
			return glm::toQuat(mat);
		}

		virtual Quaternion ToQuaternionDeg(degree angle, RotationAxis axis)
		{
			return glm::angleAxis(angle, axis);
		}

		virtual Quaternion ToQuaternionRad(rad angle, RotationAxis axis)
		{
			return glm::angleAxis(angle * RAD2DEG, axis);
		}

		virtual Quaternion ToQuaternionDeg(EulerAnglesDegree angles)
		{
			return Quaternion(angles);
		}

		virtual Quaternion ToQuaternionRad(EulerAnglesRad angles)
		{
			return Quaternion(angles * RAD2DEG);
		}

		virtual EulerAnglesDegree ToEulerAnglesDeg(Quaternion q) {
			return glm::eulerAngles(q);
		}

		virtual EulerAnglesRad ToEulerAnglesRad(Quaternion q) {
			return glm::eulerAngles(q) * DEG2RAD;
		}
	};


	
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& n);
std::ostream& operator<<(std::ostream& o, const glm::vec2& n);