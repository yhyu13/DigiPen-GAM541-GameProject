/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: FireballAttack.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#pragma once

#include "ActiveSkill.h"
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/primitives/Projectile.h"

#include <set>
#include <memory>

namespace gswy
{
	class FireballAttack: public ActiveSkill, public Projectile
	{
	public:

		FireballAttack(ActiveSkillType type);
		~FireballAttack();

		const int& GetCount();
		const glm::vec3& GetVelocity();

		virtual const int& GetId() override;
		virtual void AddSupportSkill(std::shared_ptr<SupportSkill> skill) override;

	};
}