/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EventHandler.h
Purpose			: Contains definition for event-handler.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/09/2020
- End Header ----------------------------*/


#pragma once
#include <memory>
#include "Event.h"

namespace gswy {

	class BaseEventHandler {
	
	public:
	
		void Execute(std::shared_ptr<BaseEvent> event) {
			return HandleEvent(event);
		}
	
	protected:
	
		virtual void HandleEvent(std::shared_ptr<BaseEvent> event) = 0;

	};

	template<typename T, typename EntityType, typename EventType>
	class EventHandler : public BaseEventHandler {

	public:

		typedef void (T::* HandlerFunction) (std::shared_ptr<Event<EntityType, EventType>>);

		EventHandler(T* instance, HandlerFunction function) : m_handlerOwner(instance), m_handler(function) {
		}

		virtual void HandleEvent(std::shared_ptr<BaseEvent> event) {
			(m_handlerOwner->*m_handler)(static_pointer_cast<Event<EntityType, EventType>>(event));
		}

	private:

		T* m_handlerOwner;
		HandlerFunction m_handler;

	};
}