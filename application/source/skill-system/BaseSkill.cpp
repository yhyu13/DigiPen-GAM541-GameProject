/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseSkill.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "BaseSkill.h"

#include <string>

namespace gswy
{
	int BaseSkill::m_id = 0;

	BaseSkill::BaseSkill()
	{
		++m_id;
	}

	BaseSkill::~BaseSkill()
	{
	}

	void BaseSkill::AddSkillTag(const SkillTag& tag)
	{
		m_tags.insert(tag);
	}

	void BaseSkill::RemoveSkillTag(const SkillTag& tag)
	{
		m_tags.erase(tag);
	}

	const int& BaseSkill::GetId()
	{
		return m_id;
	}

	std::set<SkillTag> BaseSkill::GetSkillTags()
	{
		return m_tags;
	}

	std::string& BaseSkill::GetIcon()
	{
		return m_icon;
	}

	void BaseSkill::SetIcon(std::string& icon)
	{
		m_icon = icon;
	}

	std::string& BaseSkill::GetIconGray()
	{
		return m_iconGray;
	}

	void BaseSkill::SetIconGray(std::string& icon)
	{
		m_iconGray = icon;
	}
}