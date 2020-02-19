/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLDebugDraw.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/17/2020
- End Header ----------------------------*/

#pragma once
#include "engine/math/MathHelper.h"

namespace gswy {

	class OpenGLDebugDraw
	{
	public:
		static void Init();
		static void End();

		static void DrawLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, glm::vec3 color, float lineWidth = 1.0f);		
		static void DrawCircle(const glm::vec3& centerPosition, float radius, glm::vec3 color);
		static void DrawAABB(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec3& color);
	};
}


