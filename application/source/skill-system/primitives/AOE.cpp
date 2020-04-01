/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/30/2020
- End Header ----------------------------*/

#include "AOE.h"

gswy::AOE::AOE() : m_multiplier(1)
{
	AddSkillTag(SkillTag::AOE);
}

gswy::AOE::~AOE()
{
}

const float& gswy::AOE::GetAOEMultipler()
{
	return m_multiplier;
}

void gswy::AOE::SetAOEMultipler(const float& multiplier)
{
	if (multiplier > 0)
	{
		m_multiplier = multiplier;
	}
}
