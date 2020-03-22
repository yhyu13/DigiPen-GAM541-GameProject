/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: FireballAttack.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "FireballAttack.h"

#include <memory>

namespace gswy
{
	FireballAttack::FireballAttack(ActiveSkillType type): ActiveSkill(type)
	{
	}

	FireballAttack::~FireballAttack()
	{
	}

	const int& FireballAttack::GetCount()
	{
		return Projectile::GetCount();
	}

	const glm::vec3& FireballAttack::GetVelocity()
	{
		return Projectile::GetVelocity();
	}

	const int& FireballAttack::GetId()
	{
		return BaseSkill::GetId();
	}

	void FireballAttack::AddSupportSkill(std::shared_ptr<SupportSkill> skill)
	{
		ActiveSkill::AddSupportSkill(skill);
		skill->HandleSkill(this);
	}

	void FireballAttack::AddSupportSkill(const int& slot, std::shared_ptr<SupportSkill> skill)
	{
		ActiveSkill::AddSupportSkill(slot, skill);
		skill->HandleSkill(this);
	}
}