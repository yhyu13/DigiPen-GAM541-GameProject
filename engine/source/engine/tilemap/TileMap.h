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

#pragma once
#include <Tileson.h>
#include "engine/allocator/MemoryManager.h"
#include "engine/math/MathHelper.h"
#include "engine/ai/Grid_float.h"
#include "engine/ai/PathFinding.h"

namespace gswy {

	extern float GSWY_GetPixel2WorldNumerator();

	/*
	Use case:
		auto tileMapObj = GameTileMapManager::GetInstance()->GetCurrentMap();
		auto pathGrid = tileMapObj->GetTileGrid("Path");
		auto Astar = tileMapObj->GetPathFinder("Path");
		if (Astar->Search(*pathGrid, _src, _dest))
		{
			auto result = Astar->GetResult();
		}
		else
		{
			...
		}
	*/
	class TileMap
	{
	public:
		typedef Grid_float TileGrid;
		typedef PathFinding PathFinder;

		explicit TileMap(const std::string& name)
			:
			m_name(name),
			m_Map(nullptr),
			m_PathFinder(nullptr)
		{
		}
		static std::shared_ptr<TileMap> Create(const std::string& path);
		std::shared_ptr<tson::Map> GetMap();
		TileGrid* GetTileGrid(const std::string& name);
		std::shared_ptr<PathFinder> GetPathFinder(const std::string& name);
		/*
			World position to tile grid position
		*/
		ivec2 World2Grid(const vec2& v);
		/*
			World position to tile pixel position
		*/
		vec2 World2Pixel(const vec2& v);
		/*
			Tile pixel position to world position
		*/
		vec2 Pixel2World(const vec2& v);
		/*
			Tile grid position to world position
		*/
		vec2 Grid2World(const ivec2& v);

	private:
		std::string m_name;
		std::shared_ptr<tson::Map> m_Map;
		std::shared_ptr<PathFinder> m_PathFinder;
		std::map<std::string, TileGrid> m_Grids;
	};
}



