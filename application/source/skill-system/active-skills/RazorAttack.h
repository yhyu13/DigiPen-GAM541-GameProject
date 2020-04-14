/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: RazorAttack.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/26/2020
- End Header ----------------------------*/

#pragma once

#include "ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/primitives/AOE.h"
#include "skill-system/primitives/Damage.h"

namespace gswy
{
	class RazorAttack : public ActiveSkill, public AOE, public Damage
	{

	public:

		RazorAttack();
		~RazorAttack();
	};
}
