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

#include "IncreaseAOE.h"
#include "skill-system/primitives/AOE.h"

gswy::IncreaseAOE::IncreaseAOE() : SupportSkill(SupportSkillType::INCREASE_AOE)
{
	m_value = 1.3;
}

gswy::IncreaseAOE::~IncreaseAOE()
{
}

const float& gswy::IncreaseAOE::GetValue()
{
	return m_value;
}

void gswy::IncreaseAOE::SetValue(const float value)
{
	m_value = value;
}

void gswy::IncreaseAOE::HandleSkill(BaseSkill* skill)
{
	if (auto s = dynamic_cast<AOE*>(skill))
	{
		s->SetAOEMultipler(m_value);
	}
	else
	{
		throw 42;
	}
}

void gswy::IncreaseAOE::RemoveSkill(std::shared_ptr<BaseSkill> skill)
{
	if (auto s = std::dynamic_pointer_cast<AOE>(skill))
	{
		s->SetAOEMultipler(1);
	}
}
