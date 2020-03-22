/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: PlayerSkillComponent.h
Purpose			: Holds data for handling player skills.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

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
		PlayerSkillComponent(): m_currentSkill(nullptr)
		{
		}

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
