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
