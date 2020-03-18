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

#include "ecs/components/ActiveCom.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/components/AttachedMovementCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/ChildrenCom.h"
#include "ecs/components/CoolDownCom.h"
#include "ecs/components/HitPointCom.h"
#include "ecs/components/HitPreventionCom.h"
#include "ecs/components/LifeTimeCom.h"
#include "ecs/components/MiniMapSprite.h"
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
			m_GCList.reserve(2048);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<GCComSys>(this, EventType::GC, &GCComSys::OnGC);
		}

		virtual void RemoveAllEntities(double dt) override {
			for (int e = (int)GameObjectType::EMPTY; e != (int)GameObjectType::NUM; ++e)
			{
				for (auto& entity : m_parentWorld->GetAllEntityWithType(GameObjectType(e)))
				{
					m_GCList.push_back(entity);
				}
			}
			GC();
		}

		void GC()
		{
			for (auto& e : m_GCList)
			{
				PRINT("Delete: " + Str(e));

				// TODO : comment this out after we solve the deallocation problem
				m_parentWorld->RemoveComponent<ActiveCom>(e);
				m_parentWorld->RemoveComponent<AnimationCom>(e);
				m_parentWorld->RemoveComponent<AttachedMovementCom>(e);
				m_parentWorld->RemoveComponent<BodyCom>(e);
				m_parentWorld->RemoveComponent<ChildrenCom<GameObjectType>>(e);
				m_parentWorld->RemoveComponent<CoolDownCom>(e);
				m_parentWorld->RemoveComponent<HitPointCom>(e);
				m_parentWorld->RemoveComponent<HitPreventionCom<GameObjectType>>(e);
				m_parentWorld->RemoveComponent<LifeTimeCom>(e);
				m_parentWorld->RemoveComponent<MiniMapSprite>(e);
				m_parentWorld->RemoveComponent<OwnershiptCom<GameObjectType>>(e);
				m_parentWorld->RemoveComponent<ParticleCom>(e);
				m_parentWorld->RemoveComponent<SpriteCom>(e);
				m_parentWorld->RemoveComponent<TransformCom>(e);

				// TODO : the goal is to call the method below to remove entity and all its components
				m_parentWorld->RemoveEntity(e);
			}
			m_GCList.clear();
		}

		virtual void PostRenderUpdate(double dt) override {
			GC();
		}

		void OnGC(EventQueue<GameObjectType, EventType>::EventPtr e)
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