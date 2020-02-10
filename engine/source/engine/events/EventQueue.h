#pragma once

#include <list>
#include <map>

#include "EventHandler.h"

namespace gswy {

	template <typename EntityType, typename EventType>
	class EventQueue {
	
	public:

		EventQueue() = default;

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
