#pragma once

#include "skill-system/BaseSkill.h"

#include <memory>

namespace gswy
{
	class SupportSkill: virtual public BaseSkill {

	public:

		SupportSkill();
		virtual ~SupportSkill();

		virtual void HandleSkill(BaseSkill* skill) = 0;
	};
}
