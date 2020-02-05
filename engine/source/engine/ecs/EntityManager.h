#pragma once

#include "engine/EngineCore.h"
#include "Entity.h"

namespace gswy {

	class ENGINE_API EntityManager {

	public:

		EntityManager();
		~EntityManager();

		const Entity Create();
		void Destroy(Entity& entity);

	protected:

	private:

		unsigned int m_entityId;
	};
}
