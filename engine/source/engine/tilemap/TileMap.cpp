/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "TileMap.h"

namespace fs = std::filesystem;

/*
	Implementation reference : https://github.com/SSBMTonberry/tileson
*/
std::shared_ptr<TileMap> gswy::TileMap::Create(const std::string& path)
{
	// TODO : Implement json serialization
	tson::Tileson parser;
	tson::Map map = parser.parse(fs::path(path));
	

	if (map.getStatus() == tson::Map::ParseStatus::OK)
	{
		auto result = MemoryManager::Make_shared<TileMap>(path);

		//You can loop through every container of objects
		for (auto& layer : map.getLayers())
		{
			if (layer.getType() == tson::Layer::Type::ObjectGroup)
			{
				// Case object layer
				for (auto& obj : layer.getObjects())
				{
					//TODO : Call an object's serializer
					auto objName = obj.getName();
					PRINT(objName);
				}
			}
			else if(layer.getType() == tson::Layer::Type::TileLayer)
			{
				// Case tile layer: scene layer, path layer
				auto layerName = layer.getName();
				PRINT(layerName);
				//When the map is of a fixed size, you can get the tiles like this
				if (!map.isInfinite())
				{
					//You can of course also loop through every tile!
					for (const auto& [pos, tile] : layer.getTileData())
					{
						//Must check for nullptr, due to how we got the first invalid tile (pos: 0, 4)
						//Would be unnecessary otherwise.
						if (tile != nullptr)
						{
#if USE_CPP17_FILESYSTEM
							fs::path imagePath;
							std::string pathStr;
#else
							std::string imagePath;
#endif
							for (auto& tileset : map.getTilesets())
							{
								if (auto temp = tileset.getTile(tile->getId()))
								{
#if USE_CPP17_FILESYSTEM
									imagePath = tileset.getImagePath();
									pathStr = imagePath.u8string();
#else
									imagePath = tileset->getImagePath();
#endif
									int firstId = tileset.getFirstgid(); //First tile id of the tileset
									int columns = tileset.getColumns(); //For the demo map it is 8.
									int rows = tileset.getTileCount() / columns;
									int lastId = (tileset.getFirstgid() + tileset.getTileCount()) - 1;

									//Get position in pixel units
									tson::Vector2i position = { std::get<0>(pos) * map.getTileSize().x,std::get<1>(pos) * map.getTileSize().y };

									int baseTilePosition = (tile->getId() - firstId); //This will determine the base position of the tile.

									//The baseTilePosition can be used to calculate offset on its related tileset image.
									int tileModX = (baseTilePosition % columns);
									int currentRow = (baseTilePosition / columns);
									int offsetX = (tileModX != 0) ? ((tileModX)*map.getTileSize().x) : (0 * map.getTileSize().x);
									int offsetY = (currentRow < rows - 1) ? (currentRow * map.getTileSize().y) :
										((rows - 1) * map.getTileSize().y);

									int width = map.getTileSize().x;
									int height = map.getTileSize().y;
								}
							}
						}	
					}
				}
			}
		}

		return result;
	}
	else if (map.getStatus() == tson::Map::ParseStatus::FileNotFound)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TJson tilemap at " + str2wstr(path) + L" does not exist!");
	}
	else if (map.getStatus() == tson::Map::ParseStatus::MissingData)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Json tilemap at " + str2wstr(path) + L" has missing data!");
	}
	else if (map.getStatus() == tson::Map::ParseStatus::ParseError)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Tileson fails to parse json tilemap at " + str2wstr(path) + L"!");
	}

	return nullptr;
}

void gswy::TileMap::AddLayer(const std::string& name, const layer_t& layer)
{
	tileLayerMap[name] = layer;
}

const gswy::TileMap::layer_t& gswy::TileMap::GetLayer(const std::string& name)
{
	auto& it = tileLayerMap.find(name);
	if (it != tileLayerMap.end())
	{
		return it->second;
	}
	else
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"The map layer " + str2wstr(name) + L" has not been added to " + str2wstr(m_name) + L"!");
	}
}
