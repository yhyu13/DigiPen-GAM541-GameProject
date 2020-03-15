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