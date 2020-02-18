/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"

namespace gswy
{
	/* Data class of LIfe Time */
	struct LifeTimeCom : BaseComponent<LifeTimeCom> {

		LifeTimeCom()
		{
			m_lifeTime = 60.0;
		}

		explicit LifeTimeCom(double lifeTime)
		{
			m_lifeTime = lifeTime;
		}

		void AddLifeTime(double lifeTime)
		{
			m_lifeTime += lifeTime;
		}

		bool IsDepleted()
		{
			return m_lifeTime <= 0.0;
		}

	private:
		double m_lifeTime;
	};
}
