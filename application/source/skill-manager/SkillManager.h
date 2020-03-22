#pragma once

#include "skill-system/active-skills/ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/support-skills/SupportSkillType.h"

#include "inventory-manager/Items.h"

#include <memory>
#include <map>
#include <string>
#include <memory>
#include <set>

#include "skill-system/support-skills/SupportSkill.h"
#include "Skill.h"

namespace gswy
{
	class SkillManager
	{

	public:

		static SkillManager* GetInstance();
		~SkillManager();

		void AddSkill(const int& skillNumber, const int& slotNumber, std::shared_ptr<Item> item);
		std::shared_ptr<Skill> GetSkill(const int& skillNumber, const int& slotNumber);

	private:

		SkillManager();

		std::shared_ptr<SupportSkill> GetSupportingSkill(const std::string& type);
		const std::string& GetSkillType(ActiveSkillType type);
		const std::string& GetSkillType(SupportSkillType type);
		std::set<std::string> GetSkillTags(std::shared_ptr<BaseSkill> skill);

	private:

		std::shared_ptr<ActiveSkill> m_skillsUpdated[4];
	};
}