#pragma once

#include <vector>

#include "Entity.h"
#include "BitMaskSignature.h"

namespace gswy {

	class GameWorld;

	class BaseComponentSystem {

	public:

		BaseComponentSystem();
		virtual ~BaseComponentSystem() = default;
		BaseComponentSystem(const BaseComponentSystem&) = default;
		BaseComponentSystem& operator=(const BaseComponentSystem&) = default;

		virtual void Init();
		virtual void Update(double frameTime);
		virtual void Render();

		void SetWorld(GameWorld* world);
		void AddEntity(const Entity& entity);
		void RemoveEntity(const Entity& entityToBeRemoved);
		BitMaskSignature& GetSystemSignature();

	protected:

		std::vector<Entity> m_registeredEntities;
		BitMaskSignature m_systemSignature;
		GameWorld* m_parentWorld;

	private:

	};
}
