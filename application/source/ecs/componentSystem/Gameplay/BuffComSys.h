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
#include "ecs/CustomEvents.h"
#include "ecs/components/BuffCom.h"

namespace gswy
{
	class BuffComSys : public BaseComponentSystem<GameObjectType> {
	public:
		BuffComSys() {
			m_systemSignature.AddComponent<BuffCom>();
		}

		virtual void Init() override 
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<BuffComSys>(this, EventType::ADD_BUFF, &BuffComSys::OnAddBuff);
			queue->Subscribe<BuffComSys>(this, EventType::REMOVE_BUFF, &BuffComSys::OnRemoveBuff);
		}

		virtual void Update(double ts) override
		{
			for (auto& entity : m_registeredEntities)
			{
				auto active = GetComponent<ActiveCom>(entity);
				if (!active->IsActive())
				{
					continue;
				}
				auto buffCom = GetComponent<BuffCom>(entity);
				buffCom->UpdateCurrentBuffs(ts);
				for (auto it = buffCom->GetCurrentBuffs().begin(); it != buffCom->GetCurrentBuffs().end(); ++it)
				{
					it->first->ApplyBuff(m_parentWorld, entity, it->second, ts);
				}
			}
		}

		void OnAddBuff(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = dynamic_pointer_cast<AddBuffEvent>(e))
			{
				auto buffCom = GetComponent<BuffCom>(event->m_entity);
				buffCom->AddBuff(event->m_buff, event->m_buff->m_duration,event->m_bReplace);
			}
		}

		void OnRemoveBuff(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = dynamic_pointer_cast<RemoveBuffEvent>(e))
			{
				auto buffCom = GetComponent<BuffCom>(event->m_entity);
				buffCom->RemoveBuff(event->m_buff);
			}
		}
	};
}