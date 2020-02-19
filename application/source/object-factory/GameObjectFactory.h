#pragma once

#include "engine/object-factory/ObjectFactory.h"

namespace gswy {

	class GameObjectFactory: public ObjectFactory {

	public:

		static GameObjectFactory* GameObjectFactory::GetInstance();

		~GameObjectFactory();

		virtual void LoadLevel(const std::string& filepath) override;

	private:

		GameObjectFactory();
	};
}
