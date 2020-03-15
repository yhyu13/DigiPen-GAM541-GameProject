#include "ActiveSkill.h"
#include "skill-system/primitives/SkillTag.h"

namespace gswy
{
	ActiveSkill::ActiveSkill()
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
		m_supportSkills.emplace_back(skill);
	}
}