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
	class GameTileMapManager : public TileMapManager
	{
	public:
		static GameTileMapManager* GetInstance()
		{
			static GameTileMapManager instance;
			return &instance;
		}

		template <typename EntityType>
		void LoadCurrentTileMap(std::shared_ptr<GameWorld<EntityType>> world)
		{
			if (m_tileMaps.find(m_currentMapName) == m_tileMaps.end())
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TileMap with name " + str2wstr(m_currentMapName) + L" has not been managed!");
			}
			tson::Map& map = m_tileMaps[m_currentMapName]->GetMap();
			//You can loop through every container of objects
			for (auto& layer : map.getLayers())
			{
				// Case object layer
				if (layer.getType() == tson::Layer::Type::ObjectGroup)
				{
					for (auto& obj : layer.getObjects())
					{
						//TODO : Call an object's serializer
						auto objName = obj.getName();
						PRINT(objName);
					}
				}
				// Case tile layer: scene layer, path layer
				else if (layer.getType() == tson::Layer::Type::TileLayer)
				{
					auto layerName = layer.getName();
					PRINT(layerName);
					//When the map is of a fixed size, you can get the tiles like this
					if (map.isInfinite())
					{
						continue;
					}

					//You can of course also loop through every tile!
					for (const auto& [pos, tile] : layer.getTileData())
					{
						//Must check for nullptr, due to how we got the first invalid tile (pos: 0, 4)
						//Would be unnecessary otherwise.
						if (tile == nullptr)
						{
							continue;
						}
						for (auto& tileset : map.getTilesets())
						{
							int firstId = tileset.getFirstgid(); //First tile id of the tileset
							int lastId = (tileset.getFirstgid() + tileset.getTileCount()) - 1;

							if (tile->getId() >= firstId && tile->getId() <= lastId)
							{
								int columns = tileset.getColumns(); 
								int rows = tileset.getTileCount() / columns;

								//Get position in pixel units
								vec2 position(std::get<0>(pos) * map.getTileSize().x, std::get<1>(pos) * map.getTileSize().y);

								int baseTilePosition = (tile->getId() - firstId); //This will determine the base position of the tile.

								//The baseTilePosition can be used to calculate offset on its related tileset image.
								int tileModX = (baseTilePosition % columns);
								int currentRow = (baseTilePosition / columns);
								int offsetX = (tileModX != 0) ? ((tileModX)*map.getTileSize().x) : (0 * map.getTileSize().x);
								int offsetY = (currentRow < rows - 1) ? (currentRow * map.getTileSize().y) :
									((rows - 1) * map.getTileSize().y);

								int width = map.getTileSize().x;
								int height = map.getTileSize().y;
								// Create background sprite for each tile
								if (layerName.compare("Background") == 0)
								{
									auto obj = world->GenerateEntity(GameObjectType::BACKGROUND);
									auto sprite = SpriteCom();
									auto m_sprite = sprite.Get();
									// Hang Yu comment : scale the tile sprite with screen height into our normalize coordinate
									m_sprite->SetSpriteScale(vec2((float)width / 360.f, (float)height / 360.f));
									m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get(tileset.getName()));
									m_sprite->SetSpriteX(offsetX);
									m_sprite->SetSpritey(offsetY);
									m_sprite->SetSpriteWidth(width);
									m_sprite->SetSpriteHeight(height);
									// Hang Yu comment : transform the tile sprite position with screen height into our normalize coordinate
									m_sprite->SetSpritePosition(vec3(position.x / 360.f, -position.y / 360.f, Z_ORDER(-1)));
									m_sprite->SetSpriteRotation(0);
									obj.AddComponent(sprite);
								}
							}
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