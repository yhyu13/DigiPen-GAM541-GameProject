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
	auto map = result->m_Map;

	if (map->getStatus() == tson::Map::ParseStatus::OK)
	{
		for (auto& tileset : map->getTilesets())
		{
			auto path = tileset.getImagePath().u8string();
			PRINT("Loading tileset at " + path);
			ResourceAllocator<Texture2D>::GetInstance()->Create(path, tileset.getName());
		}
		return result;
	}
	else if (map->getStatus() == tson::Map::ParseStatus::FileNotFound)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"TJson tilemap at " + str2wstr(path) + L" does not exist!");
	}
	else if (map->getStatus() == tson::Map::ParseStatus::MissingData)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Json tilemap at " + str2wstr(path) + L" has missing data!");
	}
	else if (map->getStatus() == tson::Map::ParseStatus::ParseError)
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Tileson fails to parse json tilemap at " + str2wstr(path) + L"!");
	}

	return nullptr;
}

std::shared_ptr<tson::Map> gswy::TileMap::GetMap()
{
	return m_Map;
}

TileGrid& gswy::TileMap::GetTileGrid(const std::string& name)
{
	if (m_Grids.find(name) == m_Grids.end())
	{
		m_Grids[name] = TileGrid(m_Map->getSize().x, m_Map->getSize().y);
	}
	return m_Grids[name];
}

ivec2 gswy::TileMap::World2Grid(const vec2& v)
{
	return ivec2(int(v.x * GSWY_GetPixel2WorldNumerator() / m_Map->getTileSize().x),
		int(-v.y * GSWY_GetPixel2WorldNumerator() / m_Map->getTileSize().y));
}

vec2 gswy::TileMap::World2Pixel(const vec2& v)
{
	return vec2(v.x * GSWY_GetPixel2WorldNumerator(),
		-v.y * GSWY_GetPixel2WorldNumerator());
}

vec2 gswy::TileMap::Pixel2World(const vec2& v)
{
	return vec2((float)v.x / GSWY_GetPixel2WorldNumerator(),
		(float)-v.y / GSWY_GetPixel2WorldNumerator());
}

vec2 gswy::TileMap::Grid2World(const vec2& v)
{
	return vec2((float)v.x * (float)m_Map->getTileSize().x / GSWY_GetPixel2WorldNumerator(),
		(float)-v.y * (float)m_Map->getTileSize().y / GSWY_GetPixel2WorldNumerator());
}