/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseComponentSystem.h
Purpose			: Contains declarations for component-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/03/2020
- End Header ----------------------------*/

#pragma once

#include <vector>

#include "Entity.h"
#include "BitMaskSignature.h"

namespace gswy {

	class GameWorld;

	/*
		Systems are the places where the actual game-code goes. The entities and
		components are simply data-storage units, systems actually work on that
		data. 

		Each system can specify which components it wishes to pay attention to.
		This information is stored in the form of a bit-mask signature. Entities
		which have the necessary components (entity's signatures matches the
		system's signature) will be registered with the system.

		Usage example:

		// declaring a component
		struct Position : gswy::BaseComponent<Position> {
			Position() = default;
			Position(float x) : x(x) {};
			float x;
		};

		// declaring a system
		class Wind : public gswy::BaseComponentSystem {
		public:
			Wind() {
				
				// wind system wants to pay attention to the Position component,
				// thus creating the signature that contains that information.
				m_systemSignature.AddComponent<Position>();
			}

			// actual game logic goes here
			virtual void Update(double frameTime) override {
			}
		};
	*/
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