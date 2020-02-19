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
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/LifeTimeCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class LifeTimeComSys : public BaseComponentSystem<GameObjectType> {
	public:
		LifeTimeComSys() {
			m_systemSignature.AddComponent<LifeTimeCom>();
		}

		virtual void Update(double dt) override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<LifeTimeCom, GameObjectType> lifeTime;
				m_parentWorld->Unpack(entity, lifeTime);
				lifeTime->AddLifeTime(-dt);
				if (lifeTime->IsDepleted())
				{
					GCEvent e(entity);
					queue->Publish(&e);
				}
			}
		}
	};
}