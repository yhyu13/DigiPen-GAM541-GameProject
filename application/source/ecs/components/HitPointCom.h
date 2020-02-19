/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/18/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"
#include "engine/exception/EngineException.h"
#include "engine/ecs/EntityDecorator.h"


namespace gswy
{
	/* Data class of LIfe Time */
	struct HitPointCom : BaseComponent<HitPointCom> {

		HitPointCom()
		{
			m_HP = 100.0;
			m_HPLock = false;
		}

		explicit HitPointCom(double HitPoint)
		{
			m_HP = HitPoint;
			m_HPLock = false;
		}

		void AddHitPoint(double HitPoint)
		{
			if (!m_HPLock) m_HP += HitPoint;
		}

		bool IsDepleted()
		{
			return m_HP <= 0.0;
		}

		void SetHitPointLock(bool b)
		{
			m_HPLock = b;
		}

	private:
		double m_HP;
		bool m_HPLock;
	};
}


