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

		virtual void LoadResources(const std::string& filepath) override;

		template <typename EntityType>
		void LoadSystem(const std::string& filepath, std::shared_ptr<GameWorld<EntityType>> m_world)
		{
			for (auto & system: GetSystems(filepath)) {
				if (system._Equal("startUp-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<StartUpControllerComSysy>());
					continue;
				}
				if (system._Equal("mainMenu-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<MainMenuControllerComSys>());
					continue;
				}
				if (system._Equal("player-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
					continue;
				}
				if (system._Equal("PlayerAnimationController")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<PlayerAnimationControllerComSys>());
					continue;
				}
				if (system._Equal("mob-1-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<Mob1ControllerComSys>());
					continue;
				}
				if (system._Equal("mob-2-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<Mob2ControllerComSys>());
					continue;
				}
				if (system._Equal("mob-1-Boss-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<Mob1BossControllerComSys>());
					continue;
				}
				if (system._Equal("mob-2-Boss-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<Mob2BossControllerComSys>());
					continue;
				}
				if (system._Equal("MobPortalControllerComSys")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<MobPortalControllerComSys>());
					continue;
				}
				if (system._Equal("tower-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<TowerControllerComSys>());
					continue;
				}
				if (system._Equal("base-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<BaseControllerComSys>());
					continue;
				}
				if (system._Equal("coin-controller")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<CoinControllerComSys>());
					continue;
				}
				if (system._Equal("scene")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
					continue;
				}
				if (system._Equal("sprite")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
					continue;
				}
				if (system._Equal("minimap")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<MiniMapSceneComSys>());
					continue;
				}
				if (system._Equal("animation")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());
					continue;
				}
				if (system._Equal("physics")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<PhysicsComSys>());
					continue;
				}
				if (system._Equal("weapon")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<WeaponComSys>());
					continue;
				}
				if (system._Equal("lifetime")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<LifeTimeComSys>());
					continue;
				}
				if (system._Equal("spawn")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<SpawningComSys>());
					continue;
				}
				if (system._Equal("sound")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<SoundComSys>());
					continue;
				}
				if (system._Equal("hitpoint")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<HitPointComSys>());
					continue;
				}
				if (system._Equal("death")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<DeathComSys>());
					continue;
				}
				if (system._Equal("fade")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<FadeComSys>());
					continue;
				}
				if (system._Equal("buff")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<BuffComSys>());
					continue;
				}
				if (system._Equal("gc")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<GCComSys>());
					continue;
				}
				if (system._Equal("attached-movement")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<AttachedMovementComSys>());
					continue;
				}
				if (system._Equal("particle")) {
					m_world->RegisterSystem(MemoryManager::Make_shared<ParticleComSys>());
					continue;
				}
				if (system._Equal("skill"))
				{
					m_world->RegisterSystem(MemoryManager::Make_shared<PlayerSkillSystem>());
					continue;
				}
				if (system._Equal("target-entity-component"))
				{
					m_world->RegisterSystem(MemoryManager::Make_shared<TargetEntityComponentSystem>());
					continue;
				}
			}
		}

		template <typename EntityType>
		void LoadLevel(const std::string& filepath, std::shared_ptr<GameWorld<EntityType>> world) 
		{
			Json::Value root;
			GetIfstream(filepath) >> root;

			for (int i = 0; i < root["entities"].size(); ++i) {
				std::string jsonPath = root["entities"][i]["file"].asString();
				Json::Value archetypeRoot;
				GetIfstream(jsonPath) >> archetypeRoot;

				std::string type = archetypeRoot["archetype"].asString();

				if (type._Equal("player")) {
					auto player = world->GenerateEntity(GameObjectType::PLAYER);
					player.AddComponent(ActiveCom());
					auto buffCom = BuffCom();
					auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.1, -1);
					buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
					player.AddComponent(buffCom);

					Json::Value components = archetypeRoot["components"];
					for (int j = 0; j < components.size(); ++j) {
						Json::Value component = components[j];
						std::string name = component["component"].asString();
						if (name._Equal("ownership")) {
							player.AddComponent(OwnershiptCom<GameObjectType>());
							continue;
						}
						if (name._Equal("transform")) {
							float x = component["x"].asFloat();
							float y = component["y"].asFloat();
							float z = component["z"].asFloat();
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
							int hp = component["value"].asInt();
							player.AddComponent(HitPointCom(hp));
							continue;
						}
						if (name._Equal("skill"))
						{
							player.AddComponent(PlayerSkillComponent());
							continue;
						}
					}

					// Create the cyclone sfx that is attached to the player but is initizied to be inactive.
					{
						auto cyclone_sfx = world->GenerateEntity(GameObjectType::CYCLONE_SFX);
						auto active = ActiveCom(false);
						cyclone_sfx.AddComponent(active);
						cyclone_sfx.AddComponent(OwnershiptCom<GameObjectType>(player));
						auto attach = AttachedMovementCom();
						attach.followPos = true;
						attach.rPos = vec2(0, 0);
						cyclone_sfx.AddComponent(attach);
						cyclone_sfx.AddComponent(TransformCom());
						auto anim = AnimationCom();
						anim.Add("Cyclone_Attack_SFX", "Move");
						anim.SetCurrentAnimationState("Move");
						cyclone_sfx.AddComponent(anim);
						auto sprite = SpriteCom();
						sprite.SetScale(vec2(0.5, 0.5));
						cyclone_sfx.AddComponent(sprite);
						auto body = BodyCom();
						body.ChooseShape("Circle", 0.5);
						cyclone_sfx.AddComponent(body);
						auto cooldown = CoolDownCom(0.15);
						cyclone_sfx.AddComponent(cooldown);
						cyclone_sfx.AddComponent(DamageCom(20));
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
