#pragma once

#include "skill-system/BaseSkill.h"
#include "skill-system/support-skills/SupportSkill.h"

#include <vector>
#include <memory>

namespace gswy
{
	class ActiveSkill : virtual public BaseSkill
	{
	public:

		ActiveSkill();
		virtual ~ActiveSkill();

		virtual const int& GetId();
		virtual void AddSupportSkill(std::shared_ptr<SupportSkill> skill);

	protected:
		std::vector<std::shared_ptr<SupportSkill>> m_supportSkills;
	};
}
