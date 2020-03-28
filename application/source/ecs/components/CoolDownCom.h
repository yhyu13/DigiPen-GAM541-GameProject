/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/11/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/exception/EngineException.h"

namespace gswy
{
	/*
	CoolDownCom for count cool down for skills
	*/
	struct CoolDownCom : BaseComponent<CoolDownCom> {

		explicit CoolDownCom(double dt)
			:
			m_period(dt),
			m_timer(0),
			m_freeze(false)
		{
		}

		void ResetCoolDown()
		{
			m_timer = 0;
		}

		void Update(double dt)
		{
			m_timer += dt;
			if (m_timer >= m_period)
			{
				m_timer = 0;
			}
		}
		
		bool IsCoolDown()
		{
			return m_timer > 0;
		}

		void SetFreeze(bool b)
		{
			m_freeze = b;
		}

		bool IsFreezed()
		{
			return m_freeze;
		}

	private:
		double m_period;
		double m_timer;
		bool m_freeze;
	};
}
