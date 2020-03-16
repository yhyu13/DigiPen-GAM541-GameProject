#pragma once

#include "ActiveSkillType.h"
#include "skill-system/BaseSkill.h"
#include "skill-system/support-skills/SupportSkill.h"

#include <set>
#include <memory>

namespace gswy
{
	class ActiveSkill : virtual public BaseSkill
	{
	public:

		ActiveSkill(ActiveSkillType type);
		virtual ~ActiveSkill();

		virtual const int& GetId();
		virtual void AddSupportSkill(std::shared_ptr<SupportSkill> skill);
		virtual ActiveSkillType GetActiveSkillType();

	private:
		std::set<std::shared_ptr<SupportSkill>> m_supportSkills;
		ActiveSkillType m_type;
	};
}
