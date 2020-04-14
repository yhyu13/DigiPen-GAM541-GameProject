/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/13/2020
- End Header ----------------------------*/

#pragma once

#include "skill-system/BaseSkill.h"

namespace gswy
{
	class Damage : virtual public BaseSkill
	{
	public:

		Damage();
		~Damage();

		void SetBaseDamange(float baseDmanage);
		const float GetDamage();
		void SetDamangeMultipler(float multiplier);
		void ApplyDamangeMultipler(float multiplier);
	private:
		float m_baseDmanage;
		float m_multiplier;
	};
}
