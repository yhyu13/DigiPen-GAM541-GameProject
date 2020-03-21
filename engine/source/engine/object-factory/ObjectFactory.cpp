/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 02/19/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "ObjectFactory.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"
#include "engine/audio/AudioManager.h"
#include "engine/renderer/Texture.h"

#include <json/json.h>

namespace gswy {

	std::map<std::string, std::ifstream> ObjectFactory::m_FileStreamMap;

	ObjectFactory* ObjectFactory::GetInstance() {
		static ObjectFactory objectFactory;
		return &objectFactory;
	}

	std::ifstream& ObjectFactory::GetIfstream(const std::string& file)
	{
		if (m_FileStreamMap.find(file) != m_FileStreamMap.end())
		{
			m_FileStreamMap[file].clear();
			m_FileStreamMap[file].seekg(0);
			return m_FileStreamMap[file];
		}
		else
		{
			m_FileStreamMap[file] = std::ifstream(file, std::ifstream::binary);
			return m_FileStreamMap[file];
		}
	}
	
	ObjectFactory::ObjectFactory() {
	}

	ObjectFactory::~ObjectFactory() {
	}

	void ObjectFactory::LoadLevel(const std::string& filepath) {
		//Json::Value root;
		//GetIfstream(filepath) >> root;
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

	void ObjectFactory::LoadResources(const std::string& filepath) {
		Json::Value root;
		GetIfstream(filepath) >> root;

		Json::Value resourceList = root["resources"];
		for (int i = 0; i < resourceList.size(); ++i) {
			Json::Value resources = resourceList[i];
			std::string type = resources["type"].asString();
			if (type._Equal("textures")) {
				Json::Value textures = resources["textures"];
				for (int j = 0; j < textures.size(); ++j) {
					std::string name = textures[j]["name"].asString();
					std::string path = textures[j]["path"].asString();
					ResourceAllocator<Texture2D>::GetInstance()->Create(path, name);
				}
				continue;
			}

			if (type._Equal("animation")) {
				Json::Value animations = resources["animations"];
				for (int j = 0; j < animations.size(); ++j) {
					std::string name = animations[j]["name"].asString();
					Json::Value properties = animations[j]["properties"];
					std::string textureName = properties["texture-name"].asString();
					int x = properties["x"].asInt();
					int y = properties["y"].asInt();
					int width = properties["width"].asInt();
					int height = properties["height"].asInt();
					float delay = properties["delay"].asFloat();
					float frameCount = properties["frame-count"].asFloat();

					if (name._Equal("PortalAnimation_0")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}
					if (name._Equal("BaseIdle")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 1; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}
					if (name._Equal("BaseAnimation")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 3; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}
					if (name._Equal("PlayerAnimation_Idle")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("PlayerAnimation_Moving")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("PlayerAnimation_Attack")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 8; ++k) {
							playerAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob1Animation_Moving")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 8; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob1Animation_Attack")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 8; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("fireBallAnim1")) {
						auto fireBallAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 1; k <= 6; ++k) {
							fireBallAnim1->AddFrame(textureName + Str(k), x, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("boltAnim1")) {
						auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 1; k <= 9; ++k) {
							boltAnim1->AddFrame(textureName + Str(k), 0, 0, 512, 512, 1.0 / 30.0);
						}
					}

					if (name._Equal("iceBallAnim1")) {
						auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 1; k <= 9; ++k) {
							boltAnim1->AddFrame(textureName + Str(k), 0, 0, 64, 64, 1.0 / 15.0);
						}
					}
					
				}
				continue;
			}

			if (type._Equal("audio")) {
				Json::Value sounds = resources["sounds"];
				for (int j = 0; j < sounds.size(); ++j) {
					Json::Value sound = sounds[j];
					std::string path = sound["path"].asString();
					bool stream = sound["stream"].asBool();
					AudioManager::GetInstance()->LoadSound(path, stream);
				}
			}
			
		}
	}

	std::vector<std::string> ObjectFactory::GetSystems(const std::string& filepath) {
		std::vector<std::string> systemList;

		Json::Value root;
		GetIfstream(filepath) >> root;

		Json::Value systems = root["systems"];
		for (int i = 0; i < systems.size(); ++i) {
			systemList.push_back(systems[i].asString());
		}

		return systemList;
	}

}