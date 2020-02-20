
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

	//void GameObjectFactory::LoadLevel(const std::string& filepath, std::shared_ptr<GameWorld<GameObjectType>> world) {
	//	Json::Value root;
	//	std::ifstream file(filepath, std::ifstream::binary);
	//	file >> root;

	//	////std::cout << std::endl << "level json loaded:\n" << root << std::endl;


	//	for (int i = 0; i < root["entities"].size(); ++i) {
	//		std::string jsonPath = root["entities"][i]["file"].asString();
	//		Json::Value archetypeRoot;
	//		std::ifstream archetypeFile(jsonPath, std::ifstream::binary);
	//		archetypeFile >> archetypeRoot;

	//		std::string type = archetypeRoot["archetype"].asString();

	//		if (type._Equal("player")) {
	//			Json::Value components = archetypeRoot["components"];
	//			for (int j = 0; j < components.size(); ++j) {
	//				Json::Value component = components[j];
	//				std::string name = component["component"].asString();
	//				if (name._Equal("ownership")) {
	//					continue;
	//				}
	//				if (name._Equal("transform")) {
	//					continue;
	//				}
	//				if (name._Equal("sprite")) {
	//					continue;
	//				}
	//				if (name._Equal("animation")) {
	//					continue;
	//				}
	//				if (name._Equal("body")) {
	//					continue;
	//				}
	//				if (name._Equal("hit-point")) {
	//					continue;
	//				}
	//			}
	//			continue;
	//		}

	//		if (type._Equal("background")) {
	//			Json::Value components = archetypeRoot["components"];
	//			for (int j = 0; j < components.size(); ++j) {
	//				Json::Value component = components[j];
	//				std::string name = component["component"].asString();
	//				if (name._Equal("transform")) {
	//					continue;
	//				}
	//				if (name._Equal("sprite")) {
	//					continue;
	//				}
	//			}
	//			continue;
	//		}

	//		// not really needed
	//		//Json::Value components = root["entities"][i]["components"];
	//		//for (int j = 0; j < components.size(); ++j) {
	//		//	//newGameObject->Serialize(components[j]);
	//		//}
	//		//newGameObject->Intialize();
	//	}
	//}
}