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
#include "TileMapManager.h"

void gswy::TileMapManager::Init() {}

void gswy::TileMapManager::Update(double deltaTime) {}

void gswy::TileMapManager::LateUpdate(double deltaTime) {}

void gswy::TileMapManager::Shutdown() {}

void gswy::TileMapManager::SetCurrentMapName(const std::string& name)
{
	if (m_tileMaps.find(name) != m_tileMaps.end())
	{
		m_currentMapName = name;
	}
	else
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"The map " + str2wstr(name) + L" has not been managed!");
	}
}

const std::string& gswy::TileMapManager::GetCurrentMapName()
{
	return m_currentMapName;
}

std::shared_ptr<TileMap> gswy::TileMapManager::GetCurrentMap()
{
	auto& it = m_tileMaps.find(m_currentMapName);
	if (it != m_tileMaps.end())
	{
		return it->second;
	}
	else
	{
		// TODO : Engine exception
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"The map " + str2wstr(m_currentMapName) + L" has not been added!");
	}
}

void gswy::TileMapManager::AddTileMap(const std::string& name)
{
	m_tileMaps[name] = ResourceAllocator<TileMap>::GetInstance()->Get(name);
}
