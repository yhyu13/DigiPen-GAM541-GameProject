/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: MultipleProjectile.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "MultipleProjectile.h"
#include "skill-system/primitives/Projectile.h"

#include <memory>

namespace gswy
{
	MultipleProjectile::MultipleProjectile() : SupportSkill(SupportSkillType::MULTIPLE_PROJECTILE), m_multiplier(2)
	{
	}

	MultipleProjectile::~MultipleProjectile()
	{
	}

	void MultipleProjectile::SetMultiplier(const int multiplier)
	{
		m_multiplier = multiplier;
	}

	const int MultipleProjectile::GetMultiplier()
	{
		return m_multiplier;
	}

	void MultipleProjectile::HandleSkill(BaseSkill* skill)
	{
		Projectile* projectile = dynamic_cast<Projectile*> (skill);
		if (projectile != nullptr)
		{
			projectile->SetCount(projectile->GetCount() * m_multiplier);
		}
	}

	void MultipleProjectile::RemoveSkill(std::shared_ptr<BaseSkill> skill)
	{
		std::shared_ptr<Projectile> projectile = std::dynamic_pointer_cast<Projectile> (skill);
		if (projectile != nullptr)
		{
			int count = projectile->GetCount() / m_multiplier;
			if (count == 0)
			{
				count = 1;
			}
			projectile->SetCount(count);
		}
	}
}