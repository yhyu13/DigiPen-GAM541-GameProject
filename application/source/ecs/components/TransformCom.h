/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include <GLFW/glfw3.h>

using namespace glm;

namespace gswy
{
	struct TransformCom : BaseComponent<TransformCom> {
		TransformCom() = default;
		explicit TransformCom(float x, float y, float z)
		{
			pos = vec3(x,y,z);
			rotation = 0;
		}
		explicit TransformCom(const vec3& v)
		{
			pos = v;
			rotation = 0;
		}
		void AddXY(const vec2& v)
		{
			pos.x += v.x;
			pos.y += v.y;
		}
		void AddXY(float x, float y)
		{
			pos.x += x;
			pos.y += y;
		}
		void SetXY(const vec2& v)
		{
			pos.x = v.x;
			pos.y = v.y;
		}
		void SetXY(float x, float y)
		{
			pos.x = x;
			pos.y = y;
		}
		vec3& GetPos()
		{
			return pos;
		}
		void SetRotation(float r)
		{
			rotation = r;
		}
		float GetRotation()
		{
			return rotation;
		}
	private:
		vec3 pos;
		float rotation;
	};
}