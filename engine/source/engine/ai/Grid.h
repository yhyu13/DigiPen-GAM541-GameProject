/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/04/2020
- End Header ----------------------------*/

#pragma once
#include "engine/allocator/MemoryManager.h"
#include "engine/math/MathHelper.h"

namespace gswy {

	class GridProxy
	{
	public:
		GridProxy() = delete;
		explicit GridProxy(float* array, int size)
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

	class Grid
	{
	public:
		Grid()
			:
			m_x(0),
			m_y(0),
			m_grid(nullptr)
		{}

		explicit Grid(int x, int y)
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
		Grid(const Grid& rhs)
		{
			auto x = rhs.m_x;
			auto y = rhs.m_y;
			m_x = x;
			m_y = y;
			m_grid = (float**)MemoryManager::Allocate(x * sizeof(float*));
			for (int i = 0; i < x; ++i)
			{
				m_grid[i] = (float*)MemoryManager::Allocate(y * sizeof(float));
				for (int j = 0; j < y; ++j)
				{
					m_grid[i][j] = rhs.m_grid[i][j];
				}
			}
		}

		Grid& operator=(const Grid& rhs)
		{
			if (this != &rhs)
			{
				this->~Grid();
				auto x = rhs.m_x;
				auto y = rhs.m_y;
				m_x = x;
				m_y = y;
				m_grid = (float**)MemoryManager::Allocate(x * sizeof(float*));
				for (int i = 0; i < x; ++i)
				{
					m_grid[i] = (float*)MemoryManager::Allocate(y * sizeof(float));
					for (int j = 0; j < y; ++j)
					{
						m_grid[i][j] = rhs.m_grid[i][j];
					}
				}
			}
			return *this;
		}

		~Grid()
		{
			if (m_grid)
			{
				for (int i = 0; i < m_x; ++i)
				{
					MemoryManager::Free(m_grid[i], sizeof(float) * m_y);
				}
				MemoryManager::Free(m_grid, sizeof(float*) * m_x);
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
		GridProxy operator[](int i)
		{
			if (i < 0 || i > m_x - 1)
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Array " + str2wstr(Str(i)) + L" out of bound!");
			}
			return GridProxy(m_grid[i], m_y);
		}

	private:
		float** m_grid;
		int m_x;
		int m_y;
	};
}



