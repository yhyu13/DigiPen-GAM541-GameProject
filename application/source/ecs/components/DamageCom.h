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

#include "engine/ecs/BaseComponent.h"

namespace gswy
{
	/* Data class of Damange (simply store the final damage that an entity could deal) */
	struct DamageCom : BaseComponent<DamageCom> {

		DamageCom()
			:
			m_damage(0)
		{
		}

		explicit DamageCom(float damage)
		{
			m_damage = damage;
		}

		void SetDamange(float damage)
		{
			m_damage = damage;
		}

		float GetDamange()
		{
			return m_damage;
		}

	private:
		float m_damage;
	};
}
