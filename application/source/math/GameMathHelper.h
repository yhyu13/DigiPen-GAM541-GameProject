/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 05/08/2020
- End Header ----------------------------*/

#pragma once
#include "engine/math/MathHelper.h"

namespace gswy
{
	class GameMathHelper : public MathHelper
	{
	public:
		GameMathHelper()
		{
			instance = this;
		}

		virtual float LookAt(const vec2& delta) override
		{
			return (delta.y) > 0 ? -atanf(delta.x / (delta.y + 1e-4f)) : PI - atanf(delta.x / (delta.y + 1e-4f));
		}

		virtual vec2 ToVec(float rotation) override
		{
			return vec2(-sinf(rotation), cosf(rotation));
		}
	};
}