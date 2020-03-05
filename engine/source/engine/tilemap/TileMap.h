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

namespace gswy {

	class TileGridProxy
	{
	public:
		TileGridProxy() = delete;
		explicit TileGridProxy(float* array, int size)
			:
			m_size(size)
		{
			m_array = array;
		}
		float& operator[](int i)
		{
			if (i < 0 || i > m_size - 1)
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Array " + str2wstr(Str(i)) + L" out of bound!");
			}
			return m_array[i];
		}
	private:
		float* m_array;
		int m_size;
	};

	class TileGrid
	{
	public:
		TileGrid() = default;
		explicit TileGrid(int x, int y)
			:
			m_x(x),
			m_y(y)
		{
			m_grid = (float**)MemoryManager::Allocate(x * sizeof(float*));
			for (int i = 0; i < x; ++i)
			{
				m_grid[i] = (float*)MemoryManager::Allocate(y * sizeof(float));
				for (int j = 0; j < y; ++j)
				{
					m_grid[i][j] = 0.0f;
				}
			}
		}
		int X()
		{
			return m_x;
		}
		int Y()
		{
			return m_y;
		}
		TileGridProxy operator[](int i)
		{
			if (i < 0 || i > m_x - 1)
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Array " + str2wstr(Str(i)) + L" out of bound!");
			}
			return TileGridProxy(m_grid[i], m_y);
		}

	private:
		float** m_grid;
		int m_x;
		int m_y;
	};

	extern float GSWY_GetPixel2WorldNumerator();

	class TileMap
	{
	public:
		explicit TileMap(const std::string& name)
			:
			m_name(name)
		{
		}
		static std::shared_ptr<TileMap> Create(const std::string& path);
		std::shared_ptr<tson::Map> GetMap();
		TileGrid& GetTileGrid(const std::string& name);

		ivec2 World2Grid(const vec2& v);
		vec2 World2Pixel(const vec2& v);
		vec2 Pixel2World(const vec2& v);
		vec2 Grid2World(const vec2& v);

	private:
		std::string m_name;
		std::shared_ptr<tson::Map> m_Map;
		std::map<std::string, TileGrid> m_Grids;
	};
}



