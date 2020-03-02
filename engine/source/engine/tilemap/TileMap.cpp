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

std::shared_ptr<TileMap> gswy::TileMap::Create(const std::string& path)
{
	// TODO : Implement json serialization
	tson::Tileson parser;
	tson::Map map = parser.parse(fs::path(path));

	if (map.getStatus() == tson::Map::ParseStatus::OK)
	{

	}
	else if (map.getStatus() == tson::Map::ParseStatus::FileNotFound)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Tileson fails to read json tilemap at " + str2wstr(path) + L"!");
	}

	return MemoryManager::Make_shared<TileMap>(path);
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
