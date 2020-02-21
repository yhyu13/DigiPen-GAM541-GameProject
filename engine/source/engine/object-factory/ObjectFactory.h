/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 02/19/2020
- End Header ----------------------------*/

#pragma once

#include "engine-precompiled-header.h"

namespace gswy {

	class ObjectFactory {

	public:

		static ObjectFactory* GetInstance();
		static std::ifstream& GetIfstream(const std::string& file);
		
		virtual ~ObjectFactory();

		virtual void LoadLevel(const std::string& filepath);
		virtual void LoadResources(const std::string& filepath);
		virtual std::vector<std::string> GetSystems(const std::string& filepath);

	protected:
		static std::map<std::string, std::ifstream> m_FileStreamMap;
		ObjectFactory();
	};
}
