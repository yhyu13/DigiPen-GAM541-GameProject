/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/29/2020
- End Header ----------------------------*/

#pragma once

#include <map>
#include <vector>
#include "engine/ecs/BaseComponent.h"
#include "buff-system/BaseBuff.h"

namespace gswy
{
	/* Data class of buff */
	struct BuffCom : BaseComponent<BuffCom> {

		typedef std::shared_ptr<BaseBuff> BuffPtr;

		BuffCom()
		{
			
		}

		const std::map<BuffPtr, double>& GetCurrentBuffs()
		{
			return m_currentBuffDuraionMap;
		}

		void UpdateCurrentBuffs(double ts)
		{
			std::vector<BuffPtr> expiredBuffs;
			for (auto it = m_currentBuffDuraionMap.begin(); it != m_currentBuffDuraionMap.end(); ++it)
			{
				// Buffs with -1 duration are permenant
				if (it->second == -1)
				{
					continue;
				}
				if ((it->second -= ts) <= 0)
				{
					expiredBuffs.push_back(it->first);
				}
			}
			for (auto& buff : expiredBuffs)
			{
				m_currentBuffDuraionMap.erase(buff);
			}
		}

		void AddBuff(BuffPtr buff, double duration, bool replace = true)
		{
			if (replace)
			{
				RemoveBuff(buff->m_type);
			}
			m_currentBuffDuraionMap[buff] = duration;
		}

		void RemoveBuff(GameBuffType type)
		{
			std::vector<BuffPtr> expiredBuffs;
			for (auto it = m_currentBuffDuraionMap.begin(); it != m_currentBuffDuraionMap.end(); ++it)
			{
				if (it->first->m_type == type)
				{
					expiredBuffs.push_back(it->first);
				}
			}
			for (auto& buff : expiredBuffs)
			{
				m_currentBuffDuraionMap.erase(buff);
			}
		}

	private:
		std::map<BuffPtr, double> m_currentBuffDuraionMap;
	};
}
