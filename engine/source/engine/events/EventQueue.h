/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EventHandler.h
Purpose			: Contains definition for event-queue.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/09/2020
- End Header ----------------------------*/

#pragma once

#include "EventHandler.h"

namespace gswy {

	template <typename EntityType, typename EventType>
	class EventQueue {
	
	public:

		static EventQueue* GetInstance()
		{
			static EventQueue instance;
			return &instance;
		}
		EventQueue() {}
		~EventQueue() {
		}

		typedef std::list<BaseEventHandler*> EventHandlerList;

		template <typename T>
		void Subscribe(T* instance, EventType eventType, void (T::*Function)(Event<EntityType, EventType>*)) {
			EventHandlerList* eventHandlers = m_subscribers[eventType];

			if (eventHandlers == nullptr) {
				eventHandlers = new EventHandlerList();
				m_subscribers[eventType] = eventHandlers;
			}

			eventHandlers->push_back(new EventHandler<T, EntityType, EventType>(instance, Function));
		}

		void Publish(Event<EntityType, EventType>* event) {
			EventHandlerList* eventHandlers = m_subscribers[event->m_type];

			if (eventHandlers == nullptr) {
				return;
			}
			DEBUG_PRINT("Publish " + Str(*event));
			for (auto& handler : *eventHandlers) {
				if (handler != nullptr) {
					handler->Execute(event);
				}
			}
		}

	private:
		std::map<EventType, EventHandlerList*> m_subscribers;
	};


}
