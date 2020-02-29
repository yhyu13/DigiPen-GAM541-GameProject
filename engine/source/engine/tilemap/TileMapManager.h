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
#include "engine/interface/IRunTimeModule.h"
#include "engine/allocator//ResouceAllocator.h"
#include "engine/allocator/MemoryManager.h"
#include "TileMap.h"

namespace gswy {
	class TileMapManager : public IRunTimeModule
	{
	public:
		static TileMapManager* GetInstance()
		{
			static TileMapManager instance;
			return &instance;
		}

		virtual void Init();;
		virtual void Update(double deltaTime);;
		virtual void LateUpdate(double deltaTime);;
		virtual void Shutdown();;

		void SetCurrentMapName(const std::string& name);
		const std::string& GetCurrentMapName();
		std::shared_ptr<TileMap> GetCurrentMap();
		void AddTileMap(const std::string& name);

	private:
		TileMapManager()
		{}
		std::string m_currentMapName;
		std::map<std::string, std::shared_ptr<TileMap>> m_tileMaps;
	};
}