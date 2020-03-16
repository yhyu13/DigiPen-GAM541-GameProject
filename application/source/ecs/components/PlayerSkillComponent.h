#pragma once

#include "engine/ecs/BaseComponent.h"
#include "skill-system/active-skills/ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"

#include <map>
#include <memory>

namespace gswy
{
	struct PlayerSkillComponent : BaseComponent<PlayerSkillComponent>
	{

		void AddSkill(std::shared_ptr<ActiveSkill> skill)
		{
			if (m_availableSkills.empty())
			{
				m_currentSkill = skill; 
			}
			m_availableSkills[skill->GetActiveSkillType()] = skill;
		}

		void RemoveSkill(const ActiveSkillType& type)
		{
			if (m_currentSkill->GetActiveSkillType() == type)
			{
				m_currentSkill = nullptr;
			}
			m_availableSkills.erase(type);
		}

		void UpgradeSkill(const ActiveSkillType& type, std::shared_ptr<SupportSkill> skill)
		{
			std::shared_ptr<ActiveSkill> activeSkill = m_availableSkills[type];
			if (activeSkill != nullptr)
			{
				activeSkill->AddSupportSkill(skill);
			}
		}

		std::shared_ptr<ActiveSkill> GetCurrentSkill()
		{
			return m_currentSkill;
		}

		void SwitchCurrentSkill()
		{
			// TODO: Temporary functionality
			if (m_availableSkills.size() > 1)
			{
				if (m_currentSkill->GetActiveSkillType() == ActiveSkillType::FIRE_BALL)
				{
					m_currentSkill = m_availableSkills[ActiveSkillType::ICE_BALL];
				}
				else
				{
					m_currentSkill = m_availableSkills[ActiveSkillType::FIRE_BALL];
				}
			}
		}

	private:
		std::map<ActiveSkillType, std::shared_ptr<ActiveSkill>> m_availableSkills;
		std::shared_ptr<ActiveSkill> m_currentSkill;
	};
}
