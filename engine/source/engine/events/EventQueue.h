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

		void Publish(Event<EntityType, EventType>* e) {
			EventHandlerList* eventHandlers = m_subscribers[e->m_type];

			if (eventHandlers == nullptr) {
				return;
			}
			DEBUG_PRINT("Publish " + Str(*e));
			for (auto& handler : *eventHandlers) {
				if (handler != nullptr) {
					handler->Execute(e);
				}
			}
		}

		template <typename T>
		void AddTimedEvent(TimedEvent<EntityType, EventType>* event, T* instance, void (T::* Function)(TimedEvent<EntityType, EventType>*)) {			
			m_timedEvents.emplace(event, new TimedEventHandler<T, EntityType, EventType>(instance, Function));
		}

		void Update(float frameTime) {
			std::map<TimedEvent<EntityType, EventType>*, BaseEventHandler*>::iterator it = m_timedEvents.begin();
			while (it != m_timedEvents.end()) {
				TimedEvent<EntityType, EventType>* event = it->first;
				event->m_time -= frameTime;
				APP_CRITICAL("Update event type: {0}", event->m_type);
				if (event->m_time < 0.0f) {
					BaseEventHandler* handler = it->second;
					handler->Execute(event);
					it = m_timedEvents.erase(it);
				}
				else {
					++it;
				}
			}
		}

	private:
		std::map<EventType, EventHandlerList*> m_subscribers;

		std::map<TimedEvent<EntityType, EventType>*, BaseEventHandler*> m_timedEvents;
	};
}