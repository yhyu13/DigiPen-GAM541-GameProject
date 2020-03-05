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
#include "engine/ai/Grid.h"

namespace gswy {

	extern float GSWY_GetPixel2WorldNumerator();

	class TileMap
	{
	public:
		typedef Grid TileGrid;

		explicit TileMap(const std::string& name)
			:
			m_name(name)
		{
		}
		static std::shared_ptr<TileMap> Create(const std::string& path);
		std::shared_ptr<tson::Map> GetMap();
		TileGrid* GetTileGrid(const std::string& name);

		ivec2 World2Grid(const vec2& v);
		vec2 World2Pixel(const vec2& v);
		vec2 Pixel2World(const vec2& v);
		vec2 Grid2World(const ivec2& v);

	private:
		std::string m_name;
		std::shared_ptr<tson::Map> m_Map;
		std::map<std::string, TileGrid> m_Grids;
	};
}



