#pragma once

#include "Event.h"

namespace gswy {

	class BaseEventHandler {
	
	public:
	
		void Execute(BaseEvent* event) {
			return HandleEvent(event);
		}
	
	protected:
	
		virtual void HandleEvent(BaseEvent* event) = 0;

	};

	template<typename T, typename EntityType, typename EventType>
	class EventHandler : public BaseEventHandler {

	public:

		typedef void (T::* HandlerFunction) (Event<EntityType, EventType>*);

		EventHandler(T* instance, HandlerFunction function) : m_handlerOwner(instance), m_handler(function) {
		}

		virtual void HandleEvent(BaseEvent* event) {
			(m_handlerOwner->*m_handler)(static_cast<Event<EntityType, EventType>*>(event));
		}

	private:

		T* m_handlerOwner;
		HandlerFunction m_handler;

	};

}
