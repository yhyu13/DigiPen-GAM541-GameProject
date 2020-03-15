#pragma once

#include "skill-system/BaseSkill.h"
#include "skill-system/support-skills/SupportSkill.h"

#include <memory>

namespace gswy
{
	class MultipleProjectile: public SupportSkill
	{
	public:
		MultipleProjectile();
		~MultipleProjectile();

		void SetCount(const int& count);
		const int& GetCount();

		void HandleSkill(BaseSkill* skill);

	private:

		int m_count;
	};
}