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

namespace gswy {
	class TileMap
	{
	public:
		explicit TileMap(const std::string& name)
			:
			m_name(name)
		{
		}
		static std::shared_ptr<TileMap> Create(const std::string& path);
		tson::Map GetMap();

	private:
		std::string m_name;
		std::shared_ptr<tson::Map> m_Map;
	};
}



