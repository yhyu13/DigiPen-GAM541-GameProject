/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Projectile.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#pragma once

#include "skill-system/BaseSkill.h"

#include <glm/glm.hpp>

namespace gswy
{
	class Projectile : virtual public BaseSkill
	{
	public:

		Projectile();
		~Projectile();

		const int GetCount();
		void SetCount(int count);

		const glm::vec3 GetVelocity();
		void SetVelocity(const glm::vec3& velocity);

	private:

		int m_count;
		glm::vec3 m_velocity;
	};
}
