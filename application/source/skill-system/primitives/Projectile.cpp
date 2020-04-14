/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Projectile.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#include "Projectile.h"
#include "SkillTag.h"

namespace gswy
{
	Projectile::Projectile() : m_count(1), m_velocity(glm::vec3(1.0f))
	{
		AddSkillTag(SkillTag::PROJECTILE);
	}
	
	Projectile::~Projectile()
	{
		RemoveSkillTag(SkillTag::PROJECTILE);
	}

	const int Projectile::GetCount()
	{
		return m_count;
	}

	void Projectile::SetCount(int count)
	{
		m_count = count;
	}

	const glm::vec3 Projectile::GetVelocity()
	{
		return m_velocity;
	}
	
	void Projectile::SetVelocity(const glm::vec3& velocity)
	{
		m_velocity = velocity;
	}

}