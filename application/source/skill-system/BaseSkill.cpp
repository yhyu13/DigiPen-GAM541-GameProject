#include "BaseSkill.h"

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
}