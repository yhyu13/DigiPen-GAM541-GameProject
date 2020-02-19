
//#include "EngineExport.h"
#include "GameObjectFactory.h"
#include "json/json.h"


namespace gswy {

	GameObjectFactory* GameObjectFactory::GetInstance() {
		static GameObjectFactory objectFactory;
		return &objectFactory;
	}

	GameObjectFactory::~GameObjectFactory() {
	}

	GameObjectFactory::GameObjectFactory() {
	}

	void GameObjectFactory::LoadLevel(const std::string& filepath) {
		Json::Value root;
		//std::ifstream file(filepath, std::ifstream::binary);
		//file >> root;
		////std::cout << std::endl << "level json loaded:\n" << root << std::endl;
		//for (int i = 0; i < root["entities"].size(); ++i) {
		//	//GameObject* newGameObject = LoadObject(root["entities"][i]["file"].asString());
		//	Json::Value components = root["entities"][i]["components"];
		//	for (int j = 0; j < components.size(); ++j) {
		//		//newGameObject->Serialize(components[j]);
		//	}
		//	//newGameObject->Intialize();
		//}
	}
}