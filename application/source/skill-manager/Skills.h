#pragma once

#include "Skill.h"

#include <memory>

namespace gswy
{
	struct Skills
	{
		int m_skillNumber;
		std::shared_ptr<Skill> m_skills[4];
	};
}