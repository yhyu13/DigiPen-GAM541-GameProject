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

#include "engine-precompiled-header.h"
#include "EventHandler.h"

#include <GLFW/glfw3.h>

namespace gswy {

	template <typename EntityType, typename EventType>
	class EventQueue {

	private:
		template <typename EntityType, typename EventType>
		struct DelayedEvent : Event<EntityType, EventType> {

			DelayedEvent() = default;
			explicit DelayedEvent(Event<EntityType, EventType>* event, float delayTime) : m_event(event), m_triggerTime(delayTime + glfwGetTime()) {
			}

			virtual ~DelayedEvent() {
			}

			Event<EntityType, EventType>* m_event;
			float m_triggerTime;
		};
	
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
		void Subscribe(T* instance, EventType eventType, void (T::* Function)(Event<EntityType, EventType>*)) {
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

		void Publish(Event<EntityType, EventType>* event, const float& delay) {
			DelayedEvent<EntityType, EventType>* delayedEvent = new DelayedEvent<EntityType, EventType>(event, delay);
			m_events.emplace(delayedEvent);
		}

		void Update(float frameTime) {
			double now = glfwGetTime();
			while (!m_events.empty()) {
				DelayedEvent<EntityType, EventType>* delayedEvent = m_events.top();
				/*
					Since the priority queue mimic min-heap, event at the top has the lowest trigger-time.
					If the current system-time is not greater than the trigger-time of element at the top,
					program counter will come out of the loop. Otherwise, the system will keep publishing
					events.
				*/
				if (now > delayedEvent->m_triggerTime) {
					Publish(delayedEvent->m_event);
					m_events.pop();
				}
				else {
					break;
				}
			}
		}

	private:
		std::map<EventType, EventHandlerList*> m_subscribers;

		struct DelayedEventComparator {
			bool operator() (const DelayedEvent<EntityType, EventType>* event1, const DelayedEvent<EntityType, EventType>* event2) {
				return event1->m_triggerTime > event2->m_triggerTime;
			}
		};

		/*
			This queue simulates a min-heap.
		*/
		std::priority_queue<DelayedEvent<EntityType, EventType>*, std::vector<DelayedEvent<EntityType, EventType>*>, DelayedEventComparator> m_events;

	};
}