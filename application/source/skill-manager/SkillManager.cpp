/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: SkillManager.cpp
Purpose			: Holds gameplay logic for handling player skills.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/20/2020
- End Header ----------------------------*/

#include "SkillManager.h"
#include "skill-system/active-skills/FireballAttack.h"
#include "skill-system/active-skills/IceballAttack.h"
#include "skill-system/support-skills/MultipleProjectile.h"
#include "skill-system/support-skills/Forking.h"
#include "skill-system/support-skills/IncreaseAOE.h"
#include "skill-system/support-skills/ConcentratedAOE.h"
#include "skill-system/active-skills/RazorAttack.h"
#include "skill-system/active-skills/CycloneAttack.h"
#include "ecs/CustomEvents.h"
#include "Skill.h"
#include "engine/allocator/MemoryManager.h"

namespace gswy
{

	SkillManager* SkillManager::GetInstance()
	{
		static SkillManager instance;
		return &instance;
	}

	SkillManager::~SkillManager()
	{
	}

	SkillManager::SkillManager()
	{
	}

	/*
		(REQUIRED : ADDING NEW SKILL)
	*/
	std::shared_ptr<SupportSkill> SkillManager::GetSupportingSkill(const std::string& type)
	{
		if (type._Equal("MULTIPLE-PROJECTILE"))
		{
			return std::make_shared<MultipleProjectile>();
		}
		if (type._Equal("FORK"))
		{
			return std::make_shared<Forking>();
		}
		if (type._Equal("INCREASE-AOE"))
		{
			return std::make_shared<IncreaseAOE>();
		}
		if (type._Equal("CONCENTRATED-AOE"))
		{
			return std::make_shared<ConcentratedAOE>();
		}
	}

	/*
		(REQUIRED : ADDING NEW SKILL)
	*/
	void SkillManager::AddSkill(const int& skillNumber, const int& slotNumber, std::shared_ptr<Item> item)
	{
		/*
			Layout of the skill-panel:

			===========================================
			SKILL-1	|	SKILL-2	|	SKILL-3	|	SKILL-4
			========|===========|===========|==========
			SLOT-1	|	SLOT-1	|	SLOT-1	|	SLOT-1	<======= ACTIVE SKILLS
			========|===========|===========|==========
			SLOT-2	|	SLOT-2	|	SLOT-2	|	SLOT-2	<======= SUPPORT SKILLS
			SLOT-3	|	SLOT-3	|	SLOT-3	|	SLOT-3	<======= SUPPORT SKILLS
			SLOT-4	|	SLOT-4	|	SLOT-4	|	SLOT-4	<======= SUPPORT SKILLS
			===========================================
		*/
		const int skill_ = skillNumber - 1;
		int slot_ = slotNumber - 1;

		if (slot_ == 0 && item->m_category._Equal("ACTIVE"))
		{
			std::shared_ptr<ActiveSkill> newSkill;
			if (item->m_type._Equal("FIRE-BALL"))
			{
				newSkill = std::make_shared<FireballAttack>(ActiveSkillType::FIRE_BALL);
			}
			else if (item->m_type._Equal("ICE-BALL"))
			{
				newSkill = std::make_shared<IceballAttack>(ActiveSkillType::ICE_BALL);
			}
			else if (item->m_type._Equal("RAZOR"))
			{
				newSkill = std::make_shared<RazorAttack>();
			}
			else if (item->m_type._Equal("CYCLONE"))
			{
				newSkill = std::make_shared<CycloneAttack>();
			}
			newSkill->SetIcon(item->m_icon);
			newSkill->SetIconGray(item->m_iconGray);
			std::shared_ptr<ActiveSkill> currentSkill = m_skills[skill_];
			if (currentSkill != nullptr)
			{

				/*
					Before over-writing an existing active skill.
					Update the new skill with support skills of the existing active skill.
				*/
				for (int slot = 0; slot < currentSkill->GetMaxSupportSKillNum(); ++slot)
				{
					std::shared_ptr<SupportSkill> supportSkill = currentSkill->GetSupportSkill(slot);
					if (supportSkill != nullptr)
					{
						newSkill->AddSupportSkill(slot, supportSkill);
					}
				}
				m_skills[skill_] = newSkill; // over-write the active skill
			}
			else
			{
				m_skills[skill_] = newSkill; // new active skill
			}

			// add key binding
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto event = MemoryManager::Make_shared<KeyBindEvent>(skillNumber, item->m_keyEventType);
			queue->Publish(event);
		}
		else if (slot_ > 0 && item->m_category._Equal("SUPPORT"))
		{
			slot_ -= 1;
			std::shared_ptr<ActiveSkill> activeSkill = m_skills[skill_];
			if (activeSkill != nullptr)
			{
				std::shared_ptr<SupportSkill> existingSupportSkill = activeSkill->GetSupportSkill(slot_);
				if (existingSupportSkill != nullptr) // a support-skill already exists at the slot 
				{
					// remove the behavior of the existing support-skill from the active-skill
					existingSupportSkill->RemoveSkill(activeSkill);
				}

				if (item->m_type._Equal("MULTIPLE-PROJECTILE"))
				{
					std::shared_ptr<SupportSkill> supportSkill = std::make_shared<MultipleProjectile>();
					supportSkill->SetIcon(item->m_icon);
					supportSkill->SetIconGray(item->m_iconGray);
					activeSkill->AddSupportSkill(slot_, supportSkill);
				}
				if (item->m_type._Equal("FORK"))
				{
					std::shared_ptr<SupportSkill> supportSkill = std::make_shared<Forking>();
					supportSkill->SetIcon(item->m_icon);
					supportSkill->SetIconGray(item->m_iconGray);
					activeSkill->AddSupportSkill(slot_, supportSkill);
				}

				if (item->m_type._Equal("INCREASE-AOE"))
				{
					std::shared_ptr<SupportSkill> supportSkill = std::make_shared<IncreaseAOE>();
					supportSkill->SetIcon(item->m_icon);
					supportSkill->SetIconGray(item->m_iconGray);
					activeSkill->AddSupportSkill(slot_, supportSkill);
				}

				if (item->m_type._Equal("CONCENTRATED-AOE"))
				{
					std::shared_ptr<SupportSkill> supportSkill = std::make_shared<ConcentratedAOE>();
					supportSkill->SetIcon(item->m_icon);
					supportSkill->SetIconGray(item->m_iconGray);
					activeSkill->AddSupportSkill(slot_, supportSkill);
				}
			}
		}
	}

	std::shared_ptr<Skill> SkillManager::GetSkill(const int& skillNumber, const int& slotNumber)
	{
		int skill_ = skillNumber - 1;
		int slot_ = slotNumber - 1;
		std::shared_ptr<Skill> skill = nullptr;
		std::shared_ptr<ActiveSkill> activeSkill = m_skills[skill_];
		if (activeSkill != nullptr)
		{
			if (slot_ == 0) // just return the information about active skill
			{
				skill = std::make_shared<Skill>();
				skill->m_category = "ACTIVE";
				skill->m_type = GetSkillType(activeSkill->GetActiveSkillType());
				skill->m_tags = GetSkillTags(activeSkill);
				skill->m_icon = activeSkill->GetIcon();
				skill->m_iconGray = activeSkill->GetIconGray();
				skill->m_skillNumber = skillNumber;
				skill->m_slotNumber = slotNumber;
			}
			else
			{
				slot_ -= 1;
				std::shared_ptr<SupportSkill> supportSkill = activeSkill->GetSupportSkill(slot_);
				if (supportSkill != nullptr)
				{
					skill = std::make_shared<Skill>();
					skill->m_category = "SUPPORT";
					skill->m_type = GetSkillType(supportSkill->GetSkillType());
					skill->m_tags = GetSkillTags(supportSkill);
					skill->m_icon = supportSkill->GetIcon();
					skill->m_iconGray = supportSkill->GetIconGray();
					skill->m_skillNumber = skillNumber;
					skill->m_slotNumber = slotNumber;
				}
			}
		}
		return skill;
	}

	std::shared_ptr<Skills> SkillManager::GetSkill(const int& skillNumber)
	{
		int skill_ = skillNumber - 1;
		std::shared_ptr<Skills> skills = std::make_shared<Skills>();
		std::shared_ptr<ActiveSkill> activeSkill = m_skills[skill_];
		if (activeSkill != nullptr)
		{
			std::shared_ptr<Skill> skill = std::make_shared<Skill>();
			skill->m_category = "ACTIVE";
			skill->m_type = GetSkillType(activeSkill->GetActiveSkillType());
			skill->m_tags = GetSkillTags(activeSkill);
			skill->m_skillNumber = skillNumber;
			skill->m_slotNumber = 1;
			skill->m_icon = activeSkill->GetIcon();
			skill->m_iconGray = activeSkill->GetIconGray();
			skills->m_skills[0] = skill;

			for (int i = 0; i < activeSkill->GetMaxSupportSKillNum(); ++i)
			{
				std::shared_ptr<SupportSkill> supportSkill = activeSkill->GetSupportSkill(i);
				if (supportSkill != nullptr)
				{
					std::shared_ptr<Skill> skill = std::make_shared<Skill>();
					skill->m_category = "SUPPORT";
					skill->m_type = GetSkillType(supportSkill->GetSkillType());
					skill->m_tags = GetSkillTags(supportSkill);
					skill->m_skillNumber = skillNumber;
					skill->m_slotNumber = i + 2;
					skills->m_skills[i + 1] = skill;
					skill->m_icon = supportSkill->GetIcon();
					skill->m_iconGray = supportSkill->GetIconGray();
				}
			}
		}
		skills->m_skillNumber = skillNumber;
		return skills;
	}

	void SkillManager::RemoveSkill(int skillNumber, int slotNumber)
	{
		// adjusting the skill number and slot number
		skillNumber -= 1;
		slotNumber -= 1;
		std::shared_ptr<ActiveSkill>& activeSkill = m_skills[skillNumber];
		if (activeSkill != nullptr)
		{
			if (slotNumber == 0) // remove the active skill
			{
				// remove all the support skills
				for (int i = 0; i < activeSkill->GetMaxSupportSKillNum(); ++i)
				{
					if (activeSkill->GetSupportSkill(i) != nullptr)
					{
						activeSkill->ResetSupportSkill(i);
					}
				}
				activeSkill.reset();
				activeSkill = nullptr;
			}
			else
			{
				// adjusting the slot number of the support skill, as the skill in slot 2 corresponds to
				// the support skill at index 0 in the support-skills array
				slotNumber -= 1;
				if (activeSkill->GetSupportSkill(slotNumber) != nullptr)
				{
					activeSkill->GetSupportSkill(slotNumber)->RemoveSkill(activeSkill); // remove behavior of support-skill from active-skill
					activeSkill->ResetSupportSkill(slotNumber); // reset the support skill
				}
			}
		}
	}

	std::shared_ptr<ActiveSkill> SkillManager::GetActiveSkill(int skillNumber)
	{
		return m_skills[skillNumber - 1];
	}

	/*
		(REQUIRED : ADDING NEW SKILL)
	*/
	std::string SkillManager::GetSkillType(ActiveSkillType type)
	{
		if (type == ActiveSkillType::FIRE_BALL)
		{
			return "FIRE-BALL";
		}

		if (type == ActiveSkillType::ICE_BALL)
		{
			return "ICE-BALL";
		}

		if (type == ActiveSkillType::RAZOR)
		{
			return "RAZOR";
		}

		if (type == ActiveSkillType::CYCLONE)
		{
			return "CYCLONE";
		}

		return "";
	}

	/*
		(REQUIRED : ADDING NEW SKILL)
	*/
	std::string SkillManager::GetSkillType(SupportSkillType type)
	{
		if (type == SupportSkillType::MULTIPLE_PROJECTILE)
		{
			return "MULTIPLE-PROJECTILE";
		}

		if (type == SupportSkillType::FORK)
		{
			return "FORK";
		}

		if (type == SupportSkillType::INCREASE_AOE)
		{
			return "INCREASE-AOE";
		}

		if (type == SupportSkillType::CONCENTRATED_AOE)
		{
			return "CONCENTRATED-AOE";
		}

		return "";
	}

	/*
		(REQUIRED : ADDING NEW SKILL)
	*/
	std::set<std::string> SkillManager::GetSkillTags(std::shared_ptr<BaseSkill> skill)
	{
		std::set<std::string> result;
		std::set<SkillTag> tags = skill->GetSkillTags();
		for (auto it = tags.begin(); it != tags.end(); ++it)
		{
			if ((*it) == SkillTag::ACTIVE)
			{
				result.emplace("ACTIVE");
			}
			if ((*it) == SkillTag::AOE)
			{
				result.emplace("AOE");
			}
			if ((*it) == SkillTag::PROJECTILE)
			{
				result.emplace("PROJECTILE");
			}
			if ((*it) == SkillTag::FORK)
			{
				result.emplace("FORK");
			}
			if ((*it) == SkillTag::SPEED)
			{
				result.emplace("SPEED");
			}
			if ((*it) == SkillTag::SUPPORT)
			{
				result.emplace("SUPPORT");
			}
		}
		return result;
	}

	std::shared_ptr<ActiveSkill> SkillManager::GetActiveSkill(ActiveSkillType type)
	{
		std::shared_ptr<ActiveSkill> skill = nullptr;
		for (int i = 0; i < 4; ++i)
		{
			if (m_skills[i] != nullptr && m_skills[i]->GetActiveSkillType() == type)
			{
				skill = m_skills[i];
			}
		}
		return skill;
	}

	void SkillManager::ResetSkills()
	{
		for (int i = 0; i < 4; ++i)
		{
			std::shared_ptr<ActiveSkill>& activeSkill = m_skills[i];
			if (activeSkill != nullptr)
			{
				for (int j = 0; j < activeSkill->GetMaxSupportSKillNum(); ++j)
				{
					std::shared_ptr<SupportSkill> supportSkill = activeSkill->GetSupportSkill(i);
					if (supportSkill != nullptr)
					{
						supportSkill.reset();
					}
				}

				activeSkill.reset();
			}
		}
	}
}