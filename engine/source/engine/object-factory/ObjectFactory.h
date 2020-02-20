#pragma once

#include "engine-precompiled-header.h"

namespace gswy {

	class ObjectFactory {

	public:

		static ObjectFactory* GetInstance();
		virtual ~ObjectFactory();

		void LoadLevel(const std::string& filepath);

		void LoadResources(const std::string& filepath);
		std::vector<std::string> GetSystems(const std::string& filepath);

	protected:

		ObjectFactory();
	};
}
