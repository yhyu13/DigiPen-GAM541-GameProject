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
	class GameTileMapManager : public TileMapManager
	{
	public:
		static GameTileMapManager* GetInstance()
		{
			static GameTileMapManager instance;
			return &instance;
		}

		/*
			This is the method to load the current TileMap into world
		*/
		template <typename EntityType>
		void LoadCurrentTileMap(std::shared_ptr<GameWorld<EntityType>> world)
		{
			if (m_tileMaps.find(m_currentMapName) == m_tileMaps.end())
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TileMap with name " + str2wstr(m_currentMapName) + L" has not been managed!");
			}

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
						PRINT("Processing " + objName);

						// TODO : need proper handle of player creation (c++ RTTR)
						if (objName.compare("Player") == 0)
						{
							ComponentDecorator<TransformCom, GameObjectType> transform;
							world->Unpack(world->GetAllEntityWithType(GameObjectType::PLAYER)[0], transform);
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							transform->SetPos(tileMapObj->Pixel2World(pixelPos));
						}
						else if (objName.compare("Mob") == 0)
						{
							auto obj = world->GenerateEntity(GameObjectType::ENEMY);
							auto active = ActiveCom();
							obj.AddComponent(active);
							obj.AddComponent(OwnershiptCom<GameObjectType>());
							auto transform = TransformCom();
							vec2 pixelPos(object.getPosition().x, object.getPosition().y);
							transform.SetPos(tileMapObj->Pixel2World(pixelPos));
							obj.AddComponent(transform);
							auto animCom2 = AnimationCom();
							animCom2.Add("MobAnimation1", "Move");
							animCom2.SetCurrentAnimationState("Move");
							obj.AddComponent(animCom2);
							auto sprite = SpriteCom();
							sprite.SetScale(vec2(0.25, 0.25 / 70 * 50));
							obj.AddComponent(sprite);
							auto aabb1 = BodyCom();
							aabb1.ChooseShape("AABB", 0.25, 0.25 / 70 * 50);
							obj.AddComponent(aabb1);
							obj.AddComponent(HitPointCom());
						}
					}
				}
				// Case tile layer: scene layer, path layer
				else if (layer.getType() == tson::Layer::Type::TileLayer)
				{
					auto layerName = layer.getName();
					PRINT("Processing " + layerName);
					//When the map is of a fixed size, you can get the tiles like this
					if (map->isInfinite())
					{
						continue;
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
						if (layerName.compare("Path") == 0)
						{
							auto pathGrid = tileMapObj->GetTileGrid("Path");
							(*pathGrid)[gridPos.x][gridPos.y] = 1.0f;
							DEBUG_PRINT(Str(gridPos.x) + " " + Str(gridPos.y));
						}

						/*
							1, Create background tiles as individual sprites (warning performance critical!)
							2, Or cheat to use a single atlas map as background but has all tiles holding a collision box.
						*/
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
		}

		/*
			This is the method to unload the current TileMap from world
		*/
		template <typename EntityType>
		void UnloadCurrentTileMap(std::shared_ptr<GameWorld<EntityType>> world)
		{
			if (m_tileMaps.find(m_currentMapName) == m_tileMaps.end())
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TileMap with name " + str2wstr(m_currentMapName) + L" has not been managed!");
			}

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
						PRINT("Processing " + objName);

						// TODO : need proper handle of player deletion (c++ RTTR)
						if (objName.compare("Player") == 0)
						{
							
						}
						else if (objName.compare("Mob") == 0)
						{
							
						}
					}
				}
			}
		}

	private:
		GameTileMapManager()
		{}
	};
}