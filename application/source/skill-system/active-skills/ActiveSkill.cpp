/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: ActiveSkill.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "ActiveSkill.h"
#include "skill-system/primitives/SkillTag.h"

namespace gswy
{
	ActiveSkill::ActiveSkill(ActiveSkillType type): m_type(type)
	{
		AddSkillTag(SkillTag::ACTIVE);
	}

	ActiveSkill::~ActiveSkill()
	{
		RemoveSkillTag(SkillTag::ACTIVE);
	}

	const int& ActiveSkill::GetId()
	{
		return BaseSkill::GetId();
	}

	void ActiveSkill::AddSupportSkill(std::shared_ptr<SupportSkill> skill)
	{
		m_supportSkills.insert(skill);
	}

	ActiveSkillType ActiveSkill::GetActiveSkillType()
	{
		return m_type;
	}

	void ActiveSkill::AddSupportSkill(const int& slot, std::shared_ptr<SupportSkill> skill)
	{
		m_suppportSkillsUpdated[slot] = skill;
	}

	std::shared_ptr<SupportSkill> ActiveSkill::GetSupportSkill(const int& slot)
	{
		return m_suppportSkillsUpdated[slot];
	}
}