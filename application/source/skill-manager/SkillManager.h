#pragma once

#include "skill-system/active-skills/ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/support-skills/SupportSkillType.h"

#include "inventory-manager/Items.h"

#include <memory>
#include <map>
#include <string>
#include <memory>

#include "skill-system/support-skills/SupportSkill.h"

namespace gswy
{
	class SkillManager
	{

	public:

		static SkillManager* GetInstance();
		~SkillManager();

		std::shared_ptr<ActiveSkill> GetCurrentSkill();
		std::map<int, std::shared_ptr<ActiveSkill>> GetAvailableSkills();


		void AddActiveSkill(std::shared_ptr<Item> item);
		void RemoveActiveSkill(const int& activeSkillId);
		void UpgradeActiveSkill(const int& activeSkillId, std::shared_ptr<Item> supportingSkill);

	private:

		SkillManager();

		std::shared_ptr<SupportSkill> GetSupportingSkill(const std::string& type);

	private:

		std::shared_ptr<ActiveSkill> m_currentSkill;

		std::map<int, std::shared_ptr<ActiveSkill>> m_availableSkills;
	};
}