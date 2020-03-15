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

		const int& GetCount();
		void SetCount(const int& count);

		const glm::vec3& GetVelocity();
		void SetVelocity(const glm::vec3& velocity);

	private:

		int m_count;
		glm::vec3 m_velocity;
	};
}
