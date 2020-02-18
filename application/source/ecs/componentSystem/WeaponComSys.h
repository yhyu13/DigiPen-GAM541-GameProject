/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/17/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class WeaponComSys : public BaseComponentSystem<GameObjectType> {
	public:
		WeaponComSys() {
		}

		virtual void Init() {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<WeaponComSys>(this, EventType::PLAYERWEAPON1, &WeaponComSys::OnPLAYERWEAPON);
			queue->Subscribe<WeaponComSys>(this, EventType::PLAYERWEAPON2, &WeaponComSys::OnPLAYERWEAPON);
			queue->Subscribe<WeaponComSys>(this, EventType::PLAYERWEAPON3, &WeaponComSys::OnPLAYERWEAPON);
		}

		void OnPLAYERWEAPON(Event<GameObjectType, EventType>* e) 
		{
			DEBUG_PRINT("Receive " + Str(*e));
		}
	};
}