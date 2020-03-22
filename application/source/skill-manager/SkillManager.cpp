#include "SkillManager.h"
#include "skill-system/active-skills/FireballAttack.h"
#include "skill-system/active-skills/IceballAttack.h"
#include "skill-system/support-skills/MultipleProjectile.h"
#include "Skill.h"

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

	std::shared_ptr<SupportSkill> SkillManager::GetSupportingSkill(const std::string& type)
	{
		if (type._Equal("MULTIPLE-PROJECTILE"))
		{
			return std::make_shared<MultipleProjectile>(SupportSkillType::MULTIPLE_PROJECTILE);
		}
	}

	void SkillManager::AddSkill(const int& skillNumber, const int& slotNumber, std::shared_ptr<Item> item)
	{
		/*
			case 1: Add a skill first time
		
		*/

		if (slotNumber == 0 && item->m_category._Equal("ACTIVE"))
		{
			std::shared_ptr<ActiveSkill> newSkill;
			if (item->m_type._Equal("FIRE-BALL"))
			{
				newSkill = std::make_shared<FireballAttack>(ActiveSkillType::FIRE_BALL);
			}
			else if (item->m_type._Equal("ICE-BALL"))
			{
				newSkill = std::make_shared<FireballAttack>(ActiveSkillType::ICE_BALL);
			}
			std::shared_ptr<ActiveSkill> currentSkill = m_skillsUpdated[skillNumber];
			if (currentSkill != nullptr)
			{
				// over-write the existing active skill
				// must call HandleSkill method for each support skill
				for (int slot = 0; slot < 3; ++slot)
				{
					std::shared_ptr<SupportSkill> supportSkill = currentSkill->GetSupportSkill(slot);
					if (supportSkill != nullptr)
					{
						newSkill->AddSupportSkill(slot, supportSkill);
					}
				}
				m_skillsUpdated[skillNumber] = newSkill;
			}
			else
			{
				// just store the active skill
				m_skillsUpdated[skillNumber] = newSkill;
			}
		}
		else if (slotNumber > 0 && item->m_category._Equal("SUPPORT"))
		{
			std::shared_ptr<ActiveSkill> activeSkill = m_skillsUpdated[skillNumber];
			if (activeSkill != nullptr)
			{
				std::shared_ptr<SupportSkill> supportSkill;
				if (item->m_type._Equal("MULTIPLE-PROJECTILE"))
				{
					supportSkill = std::make_shared<MultipleProjectile>(SupportSkillType::MULTIPLE_PROJECTILE);
					activeSkill->AddSupportSkill(slotNumber, supportSkill);
				}
			}
		}
	}

	std::shared_ptr<Skill> SkillManager::GetSkill(const int& skillNumber, const int& slotNumber)
	{
		std::shared_ptr<Skill> skill = nullptr;
		std::shared_ptr<ActiveSkill> activeSkill = m_skillsUpdated[skillNumber];
		if (activeSkill != nullptr)
		{
			if (slotNumber < 0) // just return the information about active skill
			{
				skill = std::make_shared<Skill>();
				skill->m_category = "ACTIVE";
				skill->m_type = GetSkillType(activeSkill->GetActiveSkillType());
				skill->m_skillNumber = skillNumber;
				skill->m_slotNumber = slotNumber;
				skill->m_tags = GetSkillTags(activeSkill);
			}
			else
			{
				skill = std::make_shared<Skill>();
				std::shared_ptr<SupportSkill> supportSkill = activeSkill->GetSupportSkill(slotNumber);
				skill->m_category = "SUPPORT";
				skill->m_type = GetSkillType(supportSkill->GetSkillType());
				skill->m_skillNumber = skillNumber;
				skill->m_slotNumber = slotNumber;
				skill->m_tags = GetSkillTags(supportSkill);
			}
		}
		return skill;
	}

	const std::string& SkillManager::GetSkillType(ActiveSkillType type)
	{
		if (type == ActiveSkillType::FIRE_BALL)
		{
			return "FIRE-BALL";
		}

		if (type == ActiveSkillType::ICE_BALL)
		{
			return "ICE-BALL";
		}

		return "";
	}

	const std::string& SkillManager::GetSkillType(SupportSkillType type)
	{
		if (type == SupportSkillType::MULTIPLE_PROJECTILE)
		{
			return "MULTIPLE-PROJECTILE";
		}

		return "";
	}

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
}