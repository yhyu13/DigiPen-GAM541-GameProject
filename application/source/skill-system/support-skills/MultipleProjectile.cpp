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
	MultipleProjectile::MultipleProjectile() : m_count(2)
	{
	}

	MultipleProjectile::~MultipleProjectile()
	{
	}

	void MultipleProjectile::SetCount(const int& count)
	{
		m_count = count;
	}

	const int& MultipleProjectile::GetCount()
	{
		return m_count;
	}

	void MultipleProjectile::HandleSkill(BaseSkill* skill)
	{
		Projectile* projectile = dynamic_cast<Projectile*> (skill);
		if (projectile != nullptr)
		{
			projectile->SetCount(m_count);
		}
	}
}