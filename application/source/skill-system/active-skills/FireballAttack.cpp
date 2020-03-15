#include "FireballAttack.h"

#include <memory>

namespace gswy
{
	FireballAttack::FireballAttack()
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
		return ActiveSkill::GetId();
	}

	void FireballAttack::AddSupportSkill(std::shared_ptr<SupportSkill> skill)
	{
		ActiveSkill::AddSupportSkill(skill);
		skill->HandleSkill(this);
	}
}