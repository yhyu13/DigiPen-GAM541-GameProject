/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/02/2020
- End Header ----------------------------*/

#pragma once
#include "engine/tilemap/TileMapManager.h"
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/EntityDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/math/MathHelper.h"
#include "engine/renderer/Texture.h"
#include "ecs/CustomEvents.h"
#include "ui/GameWidgetManager.h"

#define IS_INGAME (GameLevelMapManager::GetInstance()->IsInGame())

namespace gswy {
	/*
	Use case
		TileMapManager::GetInstance()->Init;
		TileMapManager::GetInstance()->Add("Level1");
		TileMapManager::GetInstance()->SetCurrentMapName("Level1");
		TileMapManager::GetInstance()->LoadCurrentTileMap(world);
		TileMapManager::GetInstance()->UnloadCurrentTileMap(world);
		...
		TileMapManager::GetInstance()->ShutDown();
	*/
	class GameLevelMapManager : public TileMapManager
	{
	public:
		static GameLevelMapManager* GetInstance()
		{
			static GameLevelMapManager instance;
			return &instance;
		}

		/*
			This is the method to load the current TileMap into world
		*/
		template<typename Enitity>
		void LoadCurrentTileMap(std::shared_ptr<GameWorld<Enitity>> world)
		{
			m_world = world;

			if (m_tileMaps.find(m_currentMapName) == m_tileMaps.end())
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TileMap with name " + str2wstr(m_currentMapName) + L" has not been managed!");
			}
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto tileMapObj = m_tileMaps[m_currentMapName];
			auto map = tileMapObj->GetMap();
			//You can loop through every container of objects
			for (auto& layer : map->getLayers())
			{
				// Case object layer
				if (layer.getType() == tson::Layer::Type::ObjectGroup)
				{
					for (auto& object : layer.getObjects())
					{
						auto objName = layer.getName();
						DEBUG_PRINT("Processing " + objName);

						// TODO : need proper handle of player creation (c++ RTTR)
						if (objName.compare("Player") == 0)
						{
							auto player = world->GetAllEntityWithType(GameObjectType::PLAYER)[0];
							ComponentDecorator<TransformCom, GameObjectType> transform;
							ComponentDecorator<BodyCom, GameObjectType> body;
							world->Unpack(player, transform);
							world->Unpack(player, body);
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							transform->SetPos(tileMapObj->Pixel2World(pixelPos));
							body->SetPos(transform->GetPos());
						}
						else if (objName.compare("Base") == 0)
						{
							auto obj = world->GenerateEntity(GameObjectType::BASE);
							auto active = ActiveCom();
							obj.AddComponent(active);
							obj.AddComponent(OwnershiptCom<GameObjectType>());
							auto transform = TransformCom();
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							transform.SetPos(tileMapObj->Pixel2World(pixelPos));
							obj.AddComponent(transform);
							auto particle = ParticleCom();
							particle.Init<ExplosionParticle>();
							obj.AddComponent(particle);
							auto animCom = AnimationCom();
							animCom.Add("BaseAnimation", "Move");
							animCom.Add("BaseIdle", "Idle");
							animCom.SetCurrentAnimationState("Move");
							obj.AddComponent(animCom);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.4, 0.4));
							obj.AddComponent(sprite);
							auto sprite0 = MiniMapSprite();
							sprite0.SetScale(vec2(0.25, 0.25));
							sprite0.SetTexture("YellowLayer");
							obj.AddComponent(sprite0);
							auto aabb = BodyCom();
							aabb.SetPos(transform.GetPos());
							aabb.ChooseShape("AABB", 0.4, 0.4);
							obj.AddComponent(aabb);
							obj.AddComponent(HitPointCom(999));
							auto buffCom = BuffCom();
							auto HPRegenBuff = MemoryManager::Make_shared<ModifyHPPercentBuff>(0.005, -1);
							buffCom.AddBuff(HPRegenBuff, HPRegenBuff->m_duration, true);
							obj.AddComponent(buffCom);
						}
						else if (objName.compare("MobSpawn1") == 0)
						{
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::ENEMY_PORTAL, vec3(tileMapObj->Pixel2World(pixelPos), 0));
							queue->Publish(e);
						}
						else if (objName.compare("Towers1") == 0)
						{
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::TOWER_BUILD, vec3(tileMapObj->Pixel2World(pixelPos), 0));
							queue->Publish(e);
						}
					}
				}
				// Case tile layer: scene layer, path layer
				else if (layer.getType() == tson::Layer::Type::TileLayer)
				{
					auto layerName = layer.getName();
					DEBUG_PRINT("Processing " + layerName);
					//When the map is of a fixed size, you can get the tiles like this
					if (map->isInfinite())
					{
						continue;
					}

					if (layerName.compare("MobPath") == 0)
					{
						auto pathGrid = tileMapObj->GetTileGrid("MobPath");
						for (int i = 0; i < pathGrid->X(); ++i)
						{
							for (int j = 0; j < pathGrid->Y(); ++j)
							{
								(*pathGrid)[i][j] = 1.0f;
							}
						}
					}
					else if (layerName.compare("PlayerBlock") == 0)
					{
						auto pathGrid = tileMapObj->GetTileGrid("PlayerBlock");
						for (int i = 0; i < pathGrid->X(); ++i)
						{
							for (int j = 0; j < pathGrid->Y(); ++j)
							{
								(*pathGrid)[i][j] = 0.0f;
							}
						}
					}

					//You can of course also loop through every tile!
					for (const auto& [pos, tile] : layer.getTileData())
					{
						//Must check for nullptr
						if (tile == nullptr)
						{
							continue;
						}

						//Get position in pixel units
						ivec2 gridPos(std::get<0>(pos), std::get<1>(pos));
						// Fill the grid for path finding.
						if (layerName.compare("MobPath") == 0)
						{
							auto pathGrid = tileMapObj->GetTileGrid("MobPath");
							(*pathGrid)[gridPos.x][gridPos.y] = 0.0f;
							DEBUG_PRINT("MobPath: " + Str(gridPos.x) + " " + Str(gridPos.y));
						}
						else if (layerName.compare("PlayerBlock") == 0)
						{
							auto pathGrid = tileMapObj->GetTileGrid("PlayerBlock");
							(*pathGrid)[gridPos.x][gridPos.y] = 1.0f;
							DEBUG_PRINT("PlayerBlock: " + Str(gridPos.x) + " " + Str(gridPos.y));
						}

						///*
						//	1, Create background tiles as individual sprites (warning performance critical!)
						//	2, Or cheat to use a single atlas map as background but has all tiles holding a collision box.
						//*/
						//for (auto& tileset : map->getTilesets())
						//{
						//	int firstId = tileset.getFirstgid(); //First tile id of the tileset
						//	int lastId = (tileset.getFirstgid() + tileset.getTileCount()) - 1;

						//	if (tile->getId() >= firstId && tile->getId() <= lastId)
						//	{
						//		int columns = tileset.getColumns(); 
						//		int rows = tileset.getTileCount() / columns;

						//		int baseTilePosition = (tile->getId() - firstId); //This will determine the base position of the tile.

						//		//The baseTilePosition can be used to calculate offset on its related tileset image.
						//		int tileModX = (baseTilePosition % columns);
						//		int currentRow = (baseTilePosition / columns);
						//		int offsetX = (tileModX != 0) ? ((tileModX)*map->getTileSize().x) : (0 * map->getTileSize().x);
						//		int offsetY = (currentRow < rows - 1) ? (currentRow * map->getTileSize().y) :
						//			((rows - 1) * map->getTileSize().y);

						//		int width = map->getTileSize().x;
						//		int height = map->getTileSize().y;

						//		// Create background sprite for each tile
						//		if (layerName.compare("Path") == 0)
						//		{
						//			// TODO: remove draw path sprites
						//			auto obj = world->GenerateEntity(GameObjectType::BACKGROUND);
						//			auto active = ActiveCom();
						//			obj.AddComponent(active);
						//			auto sprite = SpriteCom();
						//			auto m_sprite = sprite.Get();
						//			// Hang Yu comment : scale the tile sprite with screen height into our normalize coordinate
						//			m_sprite->SetSpriteScale(vec2((float)width / GSWY_GetPixel2WorldNumerator(), (float)height / GSWY_GetPixel2WorldNumerator()));
						//			m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get(tileset.getName()));
						//			m_sprite->SetSpriteX(offsetX);
						//			m_sprite->SetSpritey(offsetY);
						//			m_sprite->SetSpriteWidth(width);
						//			m_sprite->SetSpriteHeight(height);
						//			// Hang Yu comment : transform the tile sprite position with screen height into our normalize coordinate
						//			auto world_pos = vec3(tileMapObj->Grid2World(gridPos), Z_ORDER(-1));
						//			m_sprite->SetSpritePosition(world_pos);
						//			m_sprite->SetSpriteRotation(0);
						//			obj.AddComponent(sprite);
						//		}
						//	}
						//}
					}
				}
			}
			m_isAnyLevelLoaded = true;
		}

		/*
			This is the method to unload the current TileMap from world
		*/
		template<typename Enitity>
		void UnloadCurrentTileMap(std::shared_ptr<GameWorld<Enitity>> world)
		{
			m_world = world;
			m_isAnyLevelLoaded = false;
		}

		template<typename Enitity>
		void LoadLevel(std::shared_ptr<GameWorld<Enitity>> world, int level)
		{
			m_world = world;

			if (m_tileMaps.find(m_currentMapName) == m_tileMaps.end())
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TileMap with name " + str2wstr(m_currentMapName) + L" has not been managed!");
			}
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto tileMapObj = m_tileMaps[m_currentMapName];
			auto map = tileMapObj->GetMap();
			//You can loop through every container of objects
			for (auto& layer : map->getLayers())
			{
				// Case object layer
				if (layer.getType() == tson::Layer::Type::ObjectGroup)
				{
					for (auto& object : layer.getObjects())
					{
						auto objName = layer.getName();
						DEBUG_PRINT("Processing " + objName);

						if (objName.compare("MobSpawn" + Str(level)) == 0)
						{
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::ENEMY_PORTAL, vec3(tileMapObj->Pixel2World(pixelPos), 0));
							queue->Publish(e);
						}
						else if (objName.compare("Towers" + Str(level)) == 0)
						{
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							auto e = MemoryManager::Make_shared<SpawnEvent>(GameObjectType::TOWER_BUILD, vec3(tileMapObj->Pixel2World(pixelPos), 0));
							queue->Publish(e);
						}
					}
				}
			}
		}

		/*
			Is loading
		*/
		bool IsLoading();
		/*
			Set Is loading
		*/
		void SetIsLoading(bool b);

		/*
			Start the level (start the count down, mainly)
		*/
		void StartWave();

		/*
			Check if the level is started
		*/
		bool IsWaveStarted();

		/*
			Check time out
		*/
		bool IsTimeOut();

		/*
			Check if in game or not
		*/
		bool IsInGame();

		/*
			Add coins
		*/
		void AddCoins(int c);

		/*
			Get coins
		*/
		int GetCoins();

		/*
			Try Spend Coins
		*/
		bool TrySpendCoins(int amount);

		/*
			Update each frame
		*/
		void Update(double dt);

		/*
			Reset level on starting a new game
		*/
		void ResetLevelData();

		/*
			Get difficulty {0,1,2,...}
		*/
		int GetGameDifficulty();

		/*
			Set difficulty
		*/
		void SetGameDifficulty(int diff);

	private:
		/*
			A level is finished when there is no remaining enemy, this level remains started, and time is out.
		*/
		bool IsWaveFinsihed();
		/*
			Advance wave if is possible
		*/
		bool AdvanceWave();

		/*
			Advance level if is possible
		*/
		bool AdvanceLevel();

	private:
		GameLevelMapManager();
	public:
		struct LevelData
		{
			int wave_start;
			int wave_max;
			double time_per_wave;
		};

		std::shared_ptr<GameWorld<GameObjectType>> m_world;

		bool m_isAnyLevelLoaded;
		bool m_waveStart;
		bool m_timeOut;
		bool m_bIsLoading;
		int m_coins;
		int m_Kill;

		int m_currentWave;
		int m_maxWave;

		int m_currentLevel;
		int m_maxLevel;

		int m_gameDifficulty;

		double m_timePerWave;
		double m_timeRemaining;

		std::vector<LevelData> m_levelData;
	};
}