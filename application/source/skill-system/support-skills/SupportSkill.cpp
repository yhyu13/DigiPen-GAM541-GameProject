/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: SupportSkill.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "SupportSkill.h"
#include "skill-system/primitives/SkillTag.h"

namespace gswy
{
	
	SupportSkill::SupportSkill(SupportSkillType type): m_type(type)
	{
		AddSkillTag(SkillTag::SUPPORT);
	}

	SupportSkill::~SupportSkill()
	{
		RemoveSkillTag(SkillTag::SUPPORT);
	}

	SupportSkillType SupportSkill::GetSkillType()
	{
		return m_type;
	}
}