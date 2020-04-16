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

#include "GameObjectFactory.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"
#include "engine/audio/AudioManager.h"
#include "engine/renderer/Texture.h"
#include <json/json.h>


namespace gswy {

	GameObjectFactory* GameObjectFactory::GetInstance() {
		static GameObjectFactory objectFactory;
		return &objectFactory;
	}

	GameObjectFactory::~GameObjectFactory() {
	}

	void GameObjectFactory::LoadResources(const std::string& filepath)
	{
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
					if (name._Equal("Animation_Razor_Attack"))
					{
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 3; ++k)
						{
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
					if (name._Equal("PlayerAnimation_Cyclone_Attack"))
					{
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 3; ++k)
						{
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

					if (name._Equal("Cyclone_Attack_SFX")) {
						auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
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

					if (name._Equal("Mob2Animation_Moving")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob2Animation_Attack")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob_Boss1Animation_Moving")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob_Boss1Animation_Attack")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 5; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob_Boss2Animation_Moving")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("Mob_Boss2Animation_Attack")) {
						auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 5; ++k) {
							mobAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("fireBallAnim1")) {
						auto fireBallAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 6; ++k) {
							fireBallAnim1->AddFrame(textureName, x*k, y, width, height, delay / frameCount);
						}
						continue;
					}

					if (name._Equal("boltAnim1")) {
						auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 10; ++k) {
							boltAnim1->AddFrame(textureName, x*k, y, width, height, delay / frameCount);
						}
					}

					if (name._Equal("boltAnim2")) {
						auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 10; ++k) {
							boltAnim1->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
					}

					if (name._Equal("iceBallAnim1")) {
						auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 11; ++k) {
							boltAnim1->AddFrame(textureName, x*k, y, width, height, delay / frameCount);
						}
					}

					if (name._Equal("coinAnimation"))
					{
						auto coinImage = ResourceAllocator<Animation>::GetInstance()->Create("", name);
						for (int k = 0; k < 4; ++k)
						{
							coinImage->AddFrame(textureName, x * k, y, width, height, delay / frameCount);
						}
						continue;
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

	GameObjectFactory::GameObjectFactory() {
	}
}