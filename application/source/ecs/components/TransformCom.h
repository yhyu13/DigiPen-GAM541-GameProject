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

namespace gswy
{
	struct TransformCom : BaseComponent<TransformCom> {
		TransformCom() = default;
		TransformCom(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {};
		float m_x;
		float m_y;
		float m_z;
	};
}