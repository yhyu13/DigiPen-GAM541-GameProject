#include "IceballAttack.h"

#include <memory>

namespace gswy
{
	IceballAttack::IceballAttack(ActiveSkillType type) : ActiveSkill(type)
	{
	}

	IceballAttack::~IceballAttack()
	{
	}

	const int& IceballAttack::GetCount()
	{
		return Projectile::GetCount();
	}

	const glm::vec3& IceballAttack::GetVelocity()
	{
		return Projectile::GetVelocity();
	}

	const int& IceballAttack::GetId()
	{
		return BaseSkill::GetId();
	}

	void IceballAttack::AddSupportSkill(std::shared_ptr<SupportSkill> skill)
	{
		ActiveSkill::AddSupportSkill(skill);
		skill->HandleSkill(this);
	}
}