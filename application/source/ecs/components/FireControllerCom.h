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
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"
#include "engine/exception/EngineException.h"

namespace gswy
{
	/*
	Data class that stores references to the current animation and maintain an animation state map
	*/
	struct FireControllerCom : BaseComponent<FireControllerCom> {

		explicit FireControllerCom(double dt)
			:
			m_period(dt),
			m_timer(0)
		{
		}

		void Update(double dt)
		{
			m_timer += dt;
			if (m_timer > m_period)
			{
				m_timer = 0;
			}
		}
		
		bool isCoolDown()
		{
			return m_timer != 0;
		}

	private:
		double m_period;
		double m_timer;
	};
}
