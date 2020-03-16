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
}