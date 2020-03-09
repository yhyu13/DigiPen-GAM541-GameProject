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

#include "ecs/components/AnimationCom.h"
#include "ecs/components/AttachedMovementCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/HitPointCom.h"
#include "ecs/components/HitPreventionCom.h"
#include "ecs/components/LifeTimeCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/ParticleCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/components/TransformCom.h"

namespace gswy
{
	class GCComSys : public BaseComponentSystem<GameObjectType> {
	public:
		GCComSys() {
		}

		virtual void Init() override {
			m_GCList.reserve(1024);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<GCComSys>(this, EventType::GC, &GCComSys::OnGC);
		}

		virtual void PostRenderUpdate(double dt) override {
			for (auto& e : m_GCList)
			{
				PRINT(e);

				// TODO : comment this out after we solve the deallocation problem
				m_parentWorld->RemoveComponent<AnimationCom>(e);
				m_parentWorld->RemoveComponent<AttachedMovementCom>(e);		
				m_parentWorld->RemoveComponent<BodyCom>(e);
				m_parentWorld->RemoveComponent<HitPointCom>(e);
				m_parentWorld->RemoveComponent<HitPreventionCom<GameObjectType>>(e);
				m_parentWorld->RemoveComponent<LifeTimeCom>(e);
				m_parentWorld->RemoveComponent<OwnershiptCom<GameObjectType>>(e);
				m_parentWorld->RemoveComponent<ParticleCom>(e);
				m_parentWorld->RemoveComponent<SpriteCom>(e);
				m_parentWorld->RemoveComponent<TransformCom>(e);

				// TODO : the goal is to call the method below to remove entity and all its components
				m_parentWorld->RemoveEntity(e);
			}
			m_GCList.clear();
		}

		void OnGC(EventQueue<GameObjectType, EventType>::EventPtrType e)
		{
			if (auto event = static_pointer_cast<GCEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				m_GCList.push_back(event->m_entity);
			}
		}
	private:
		std::vector<Entity<GameObjectType>> m_GCList;
	};
}