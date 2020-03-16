#pragma once

#include "ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/primitives/Projectile.h"

#include <set>
#include <memory>

namespace gswy
{
	class IceballAttack : public ActiveSkill, public Projectile
	{
	public:

		IceballAttack(ActiveSkillType type);
		~IceballAttack();

		const int& GetCount();
		const glm::vec3& GetVelocity();

		virtual const int& GetId() override;
		virtual void AddSupportSkill(std::shared_ptr<SupportSkill> skill) override;

	};
}