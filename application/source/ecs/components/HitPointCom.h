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
			m_MaxHP = m_HP;
			m_HPLock = false;
			m_IsDead = false;
		}

		explicit HitPointCom(double HitPoint)
		{
			m_HP = HitPoint;
			m_MaxHP = m_HP;
			m_HPLock = false;
			m_IsDead = false;
		}

		void AddHitPoint(double HitPoint)
		{
			if (!m_HPLock && (m_HP + HitPoint <= m_MaxHP)) m_HP += HitPoint;
		}

		bool IsDepleted()
		{
			return m_HP <= 0.0;
		}

		void SetHitPointLock(bool b)
		{
			m_HPLock = b;
		}

		void SetIsDead(bool b)
		{
			m_IsDead = b;
		}

		bool IsDead()
		{
			return m_IsDead;
		}

		double GetPercentageHP()
		{
			return (m_HP > 0)? m_HP / m_MaxHP: 0;
		}

		double GetHP()
		{
			return m_HP;
		}

	private:
		double m_HP;
		double m_MaxHP;
		bool m_HPLock;
		bool m_IsDead;
	};
}


