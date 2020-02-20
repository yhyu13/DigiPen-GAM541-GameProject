
//#include "EngineExport.h"
#include "GameObjectFactory.h"


namespace gswy {

	GameObjectFactory* GameObjectFactory::GetInstance() {
		static GameObjectFactory objectFactory;
		return &objectFactory;
	}

	GameObjectFactory::~GameObjectFactory() {
	}

	GameObjectFactory::GameObjectFactory() {
	}
}