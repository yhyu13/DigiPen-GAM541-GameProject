#include "SkillManager.h"
#include "skill-system/active-skills/FireballAttack.h"
#include "skill-system/active-skills/IceballAttack.h"
#include "skill-system/support-skills/MultipleProjectile.h"

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

	std::shared_ptr<ActiveSkill> SkillManager::GetCurrentSkill()
	{
		return m_currentSkill;
	}

	std::map<int, std::shared_ptr<ActiveSkill>> SkillManager::GetAvailableSkills()
	{
		return m_availableSkills;
	}

	void SkillManager::AddActiveSkill(std::shared_ptr<Item> item)
	{
		std::shared_ptr<ActiveSkill> skill;
		ActiveSkillType type;
		if (item->m_type._Equal("FIRE_BALL"))
		{
			type = ActiveSkillType::FIRE_BALL;
			skill = std::make_shared<FireballAttack>(type);
		}
		else if (item->m_type._Equal("ICE_BALL"))
		{
			type = ActiveSkillType::ICE_BALL;
			skill = std::make_shared<IceballAttack>(type);
		}

		if (m_availableSkills.empty())
		{
			m_currentSkill = skill;
		}
		m_availableSkills[skill->GetId()] = skill;
	}

	void SkillManager::RemoveActiveSkill(const int& activeSkillId)
	{
		m_availableSkills.erase(activeSkillId);
	}

	void SkillManager::UpgradeActiveSkill(const int& activeSkillId, std::shared_ptr<Item> supportingItem)
	{
		std::shared_ptr<ActiveSkill> activeSkill = m_availableSkills[activeSkillId];
		if (activeSkill != nullptr)
		{
			std::shared_ptr<SupportSkill> supportingSkill = GetSupportingSkill(supportingItem->m_type);
			activeSkill->AddSupportSkill(supportingSkill);
		}
	}

	std::shared_ptr<SupportSkill> SkillManager::GetSupportingSkill(const std::string& type)
	{
		if (type._Equal("MULTIPLE-PROJECTILE"))
		{
			return std::make_shared<MultipleProjectile>();
		}
	}
}