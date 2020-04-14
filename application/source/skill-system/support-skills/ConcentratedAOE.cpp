/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/13/2020
- End Header ----------------------------*/

#include "ConcentratedAOE.h"
#include "skill-system/primitives/AOE.h"
#include "skill-system/primitives/Damage.h"

gswy::ConcentratedAOE::ConcentratedAOE()
	:
	SupportSkill(SupportSkillType::CONCENTRATED_AOE)
{
	m_DamageMultiplier = 1.5;
	m_AOEMultiplier = 0.7;
}

gswy::ConcentratedAOE::~ConcentratedAOE()
{
}

const float gswy::ConcentratedAOE::GetValueDamageMultiplier()
{
	return m_DamageMultiplier;
}

void gswy::ConcentratedAOE::SetValueDamageMultiplier(const float value)
{
	m_DamageMultiplier = value;
}

const float gswy::ConcentratedAOE::GetValueAOEMultiplier()
{
	return m_AOEMultiplier;
}

void gswy::ConcentratedAOE::SetValueAOEMultiplier(const float value)
{
	m_AOEMultiplier = value;
}

void gswy::ConcentratedAOE::HandleSkill(BaseSkill* skill)
{
	auto s1 = dynamic_cast<AOE*>(skill);
	auto s2 = dynamic_cast<Damage*>(skill);
	if (s1 && s2)
	{
		s1->ApplyAOEMultipler(m_AOEMultiplier);
		s2->ApplyDamangeMultipler(m_DamageMultiplier);
	}
}

void gswy::ConcentratedAOE::RemoveSkill(std::shared_ptr<BaseSkill> skill)
{
	auto s1 = std::dynamic_pointer_cast<AOE>(skill);
	auto s2 = std::dynamic_pointer_cast<Damage>(skill);
	if (s1 && s2)
	{
		s1->ApplyAOEMultipler(1.0f / m_AOEMultiplier);
		s2->ApplyDamangeMultipler(1.0f / m_DamageMultiplier);
	}
}
