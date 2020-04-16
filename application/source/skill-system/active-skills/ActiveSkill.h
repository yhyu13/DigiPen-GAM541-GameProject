/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: ActiveSkill.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

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

		virtual const int GetId();
		virtual void AddSupportSkill(std::shared_ptr<SupportSkill> skill);
		virtual ActiveSkillType GetActiveSkillType();

		virtual void AddSupportSkill(const int& slot, std::shared_ptr<SupportSkill> skill);
		virtual std::shared_ptr<SupportSkill> GetSupportSkill(const int& slot);
		virtual void ResetSupportSkill(const int& slot);
		virtual bool HasSupportSkill(const SupportSkillType& type);
		int GetMaxSupportSKillNum();

	private:
		std::set<std::shared_ptr<SupportSkill>> m_supportSkillsLegacy;
		ActiveSkillType m_type;
		std::shared_ptr<SupportSkill> m_suppportSkills[4];
		int m_maxSupportSKillNum = {4};
	};
}
