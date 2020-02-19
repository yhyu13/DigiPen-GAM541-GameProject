/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/16/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/EntityDecorator.h"

namespace gswy
{
	/*
	Data class that stores references to owner entity type
	*/
	template<typename EntityType>
	struct HitPreventionCom : BaseComponent<HitPreventionCom<EntityType>> {

		HitPreventionCom()
		{
		}

		void Add(const Entity<EntityType>& e)
		{
			vec.push_back(e);
		}

		bool IsIncluded(const Entity<EntityType>& e) {
			if (vec.empty())
			{
				return false;
			}	
			else
			{
				return std::find(vec.begin(), vec.end(), e) != vec.end();
			}
		}
	private:
		std::vector<Entity<EntityType>> vec;
	};
}
