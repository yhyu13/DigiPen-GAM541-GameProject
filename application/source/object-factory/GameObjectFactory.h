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

#pragma once

#include "engine/object-factory/ObjectFactory.h"
#include "engine/ecs/GameWorld.h"
#include "json/json.h"

namespace gswy {

	class GameObjectFactory: public ObjectFactory {

	public:

		static GameObjectFactory* GameObjectFactory::GetInstance();

		~GameObjectFactory();

		template <typename EntityType>
		void LoadLevel(const std::string& filepath, std::shared_ptr<GameWorld<EntityType>> world) {
			Json::Value root;
			GetIfstream(filepath) >> root;

			for (int i = 0; i < root["entities"].size(); ++i) {
				std::string jsonPath = root["entities"][i]["file"].asString();
				Json::Value archetypeRoot;
				GetIfstream(jsonPath) >> archetypeRoot;

				std::string type = archetypeRoot["archetype"].asString();

				if (type._Equal("player")) {
					auto player = world->GenerateEntity(GameObjectType::PLAYER);
					auto active = ActiveCom();
					player.AddComponent(active);
					Json::Value components = archetypeRoot["components"];
					for (int j = 0; j < components.size(); ++j) {
						Json::Value component = components[j];
						std::string name = component["component"].asString();
						if (name._Equal("ownership")) {
							player.AddComponent(OwnershiptCom<GameObjectType>());
							continue;
						}
						if (name._Equal("transform")) {
							int x = component["x"].asInt();
							int y = component["y"].asInt();
							int z = component["z"].asInt();
							player.AddComponent(TransformCom(x, y, Z_ORDER(z)));
							continue;
						}
						if (name._Equal("sprite")) {
							float scale = component["scale"].asFloat();
							int width = component["width"].asInt();
							int height = component["height"].asInt();
							auto sprite0 = SpriteCom();
							sprite0.SetScale(vec2(scale, scale / width * height));
							player.AddComponent(sprite0);
							continue;
						}
						if (name._Equal("miniMapSprite")) {
							auto texture = component["texture"].asString();
							float scale = component["scale"].asFloat();
							int width = component["width"].asInt();
							int height = component["height"].asInt();
							auto sprite0 = MiniMapSprite();
							sprite0.SetScale(vec2(scale, scale / width * height));
							sprite0.SetTexture(texture);
							player.AddComponent(sprite0);
							continue;
						}
						if (name._Equal("animation")) {
							auto animCom1 = AnimationCom();
							Json::Value animation = component["animation"];
							Json::Value animationNames = animation["name"];
							Json::Value states = animation["states"];
							for (int k = 0; k < states.size(); ++k) {
								animCom1.Add(animationNames[k].asString(), states[k].asString());
							}
							animCom1.SetCurrentAnimationState(animation["current-state"].asString());
							player.AddComponent(animCom1);
							continue;
						}
						if (name._Equal("body")) {
							Json::Value shape = component["shape"];
							std::string shapeType = shape["type"].asString();
							float scale = shape["scale"].asFloat();
							int width = shape["width"].asInt();
							int height = shape["height"].asInt();

							auto aabb1 = BodyCom();
							aabb1.ChooseShape(shapeType, scale, scale / width * height);
							player.AddComponent(aabb1);
							continue;
						}
						if (name._Equal("hit-point")) {
							player.AddComponent(HitPointCom());
							continue;
						}
						if (name._Equal("skill"))
						{
							player.AddComponent(PlayerSkillComponent());
							continue;
						}
					}
					continue;
				}

				if (type._Equal("background")) {
					auto background = world->GenerateEntity(GameObjectType::BACKGROUND);
					auto active = ActiveCom();
					background.AddComponent(active);
					Json::Value components = archetypeRoot["components"];
					for (int j = 0; j < components.size(); ++j) {
						Json::Value component = components[j];
						std::string name = component["component"].asString();
						if (name._Equal("transform")) {
							int x = component["x"].asInt();
							int y = component["y"].asInt();
							float z = component["z"].asFloat();
							background.AddComponent(TransformCom(x, y, z));
							continue;
						}
						if (name._Equal("sprite")) {
							std::string texture = component["texture"].asString();
							float scale = component["scale"].asFloat();
							int width = component["width"].asInt();
							int height = component["height"].asInt();
							auto sprite0 = SpriteCom();
							sprite0.SetTexture(texture);
							sprite0.SetScale(vec2(scale));
							background.AddComponent(sprite0);
							continue;
						}
					}
					continue;
				}
			}
		}

	private:

		GameObjectFactory();
	};
}
