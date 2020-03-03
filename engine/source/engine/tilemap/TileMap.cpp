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
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Texture.h"

namespace fs = std::filesystem;
/*
	Implementation reference : https://github.com/SSBMTonberry/tileson
*/
std::shared_ptr<TileMap> gswy::TileMap::Create(const std::string& path)
{
	tson::Tileson parser;
	auto result = MemoryManager::Make_shared<TileMap>(path);

	// Hang Yu comment:
	// I encountered a strange bug where using MemoryManager:Make_shared to create the tson::Map
	// would cause a faliure in copy construction. The effect was a randomness of loss of data because
	// the compiler's creation of the copy constructor of tson::Map is not complete.
	// Using std::make_shared solves this problem.
	// The strange part is that it happens in release mode where MemoryManager:Make_shared simply wraps std::make_shared
	result->m_Map = std::make_shared<tson::Map>(parser.parse(fs::path(path)));
	tson::Map& map = *(result->m_Map);

	if (map.getStatus() == tson::Map::ParseStatus::OK)
	{
		for (auto& tileset : map.getTilesets())
		{
			auto path = tileset.getImagePath().u8string();
			PRINT("Loading tileset at " + path);
			ResourceAllocator<Texture2D>::GetInstance()->Create(path, tileset.getName());
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

tson::Map gswy::TileMap::GetMap()
{
	return *m_Map;
}
