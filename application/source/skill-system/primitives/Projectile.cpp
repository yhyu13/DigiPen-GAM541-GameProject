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

	const int& Projectile::GetCount()
	{
		return m_count;
	}

	void Projectile::SetCount(const int& count)
	{
		m_count = count;
	}

	const glm::vec3& Projectile::GetVelocity()
	{
		return m_velocity;
	}
	
	void Projectile::SetVelocity(const glm::vec3& velocity)
	{
		m_velocity = velocity;
	}

}