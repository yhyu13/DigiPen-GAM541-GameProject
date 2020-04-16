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
#include "engine/exception/EngineException.h"

namespace gswy
{
	ActiveSkill::ActiveSkill(ActiveSkillType type): m_type(type)
	{
		AddSkillTag(SkillTag::ACTIVE);
		for (int i = 0; i < m_maxSupportSKillNum; ++i)
		{
			m_suppportSkills[i] = nullptr;
		}
	}

	ActiveSkill::~ActiveSkill()
	{
		RemoveSkillTag(SkillTag::ACTIVE);
	}

	const int ActiveSkill::GetId()
	{
		return BaseSkill::GetId();
	}

	void ActiveSkill::AddSupportSkill(std::shared_ptr<SupportSkill> skill)
	{
		m_supportSkillsLegacy.insert(skill);
		skill->HandleSkill(this);
	}

	ActiveSkillType ActiveSkill::GetActiveSkillType()
	{
		return m_type;
	}

	void ActiveSkill::AddSupportSkill(const int& slot, std::shared_ptr<SupportSkill> skill)
	{
		m_suppportSkills[slot] = skill;
		skill->HandleSkill(this);
	}

	std::shared_ptr<SupportSkill> ActiveSkill::GetSupportSkill(const int& slot)
	{
		if (0 <= slot && slot < m_maxSupportSKillNum)
		{
			return m_suppportSkills[slot];
		}
		else
		{
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Support skill slot " + str2wstr(Str(slot)) + L" is beyond range!");
		}
	}

	void ActiveSkill::ResetSupportSkill(const int& slot)
	{
		if (0 <= slot && slot < m_maxSupportSKillNum)
		{
			m_suppportSkills[slot].reset();
			m_suppportSkills[slot] = nullptr;
		}
		else
		{
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Support skill slot " + str2wstr(Str(slot)) + L" is beyond range!");
		}
	}

	bool ActiveSkill::HasSupportSkill(const SupportSkillType& type)
	{
		for (int i = 0; i < m_maxSupportSKillNum; ++i)
		{
			std::shared_ptr<SupportSkill> skill = m_suppportSkills[i];
			if (type == skill->GetSkillType())
			{
				return true;
			}
		}
		return false;
	}
	int ActiveSkill::GetMaxSupportSKillNum()
	{
		return m_maxSupportSKillNum;
	}
}