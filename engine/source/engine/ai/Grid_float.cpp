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

#include "engine-precompiled-header.h"
#include "Grid_float.h"

gswy::GridProxy::GridProxy(float* array, int size)
	:
	m_size(size)
{
	m_array = array;
}

gswy::Grid_float::Grid_float()
	:
	m_x(0),
	m_y(0),
	m_grid(nullptr)
{}

gswy::Grid_float::Grid_float(int x, int y)
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

gswy::Grid_float::Grid_float(const Grid_float& rhs)
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

gswy::Grid_float::~Grid_float()
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

int gswy::Grid_float::X()
{
	return m_x;
}

int gswy::Grid_float::Y()
{
	return m_y;
}
