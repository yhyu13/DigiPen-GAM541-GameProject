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
	/* Data class of sprite */
	struct AttachedMovementCom : BaseComponent<AttachedMovementCom> {
		bool followPos = false;
		bool followRot = false;
		bool followVelocity = false;
		vec2 rPos = vec2(0);
		vec2 rVelocity = vec2(0);
		float rRot = 0;
	};
}
