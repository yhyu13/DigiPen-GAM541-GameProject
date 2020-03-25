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

#include "engine-precompiled-header.h"
#include "ObjectFactory.h"
#include <json/json.h>

namespace gswy {

	std::map<std::string, std::ifstream> ObjectFactory::m_FileStreamMap;

	ObjectFactory* ObjectFactory::GetInstance() {
		static ObjectFactory objectFactory;
		return &objectFactory;
	}

	std::ifstream& ObjectFactory::GetIfstream(const std::string& file)
	{
		if (m_FileStreamMap.find(file) != m_FileStreamMap.end())
		{
			m_FileStreamMap[file].clear();
			m_FileStreamMap[file].seekg(0);
			return m_FileStreamMap[file];
		}
		else
		{
			m_FileStreamMap[file] = std::ifstream(file, std::ifstream::binary);
			return m_FileStreamMap[file];
		}
	}
	
	ObjectFactory::ObjectFactory() {
	}

	ObjectFactory::~ObjectFactory() {
	}

	void ObjectFactory::LoadLevel(const std::string& filepath) {

	}

	void ObjectFactory::LoadResources(const std::string& filepath) {

	}

	std::vector<std::string> ObjectFactory::GetSystems(const std::string& filepath) {
		std::vector<std::string> systemList;

		Json::Value root;
		GetIfstream(filepath) >> root;

		Json::Value systems = root["systems"];
		for (int i = 0; i < systems.size(); ++i) {
			systemList.push_back(systems[i].asString());
		}

		return systemList;
	}

}