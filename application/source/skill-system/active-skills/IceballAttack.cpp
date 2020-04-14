/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: IceballAttack.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "IceballAttack.h"

#include <memory>

namespace gswy
{
	IceballAttack::IceballAttack(ActiveSkillType type) : ActiveSkill(type)
	{
		SetBaseDamange(10);
	}

	IceballAttack::~IceballAttack()
	{
	}

	const int IceballAttack::GetCount()
	{
		return Projectile::GetCount();
	}

	const glm::vec3 IceballAttack::GetVelocity()
	{
		return Projectile::GetVelocity();
	}

	const int IceballAttack::GetId()
	{
		return BaseSkill::GetId();
	}
}