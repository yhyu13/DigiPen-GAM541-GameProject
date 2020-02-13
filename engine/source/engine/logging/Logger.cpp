/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Logger.cpp
Purpose			: Contains definitions for logger.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/10/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Logger.h"

namespace gswy {

	std::shared_ptr<spdlog::logger> Logger::s_engineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_applicationLogger;

	void Logger::Init() {

		spdlog::set_pattern("%^[%T] %n: %v%$");
	
		s_engineLogger = spdlog::stdout_color_mt("ENGINE");
		s_engineLogger->set_level(spdlog::level::trace);

		s_applicationLogger = spdlog::stdout_color_mt("APPLICATION");
		s_applicationLogger->set_level(spdlog::level::trace);
	}

}