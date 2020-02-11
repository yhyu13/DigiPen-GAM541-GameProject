/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Logger.h
Purpose			: Contains declarations for Logger.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/10/2020
- End Header ----------------------------*/

#pragma once


#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace gswy {

	class Logger {

	public:

		Logger() {
		};

		~Logger() {
		}

		static void Init();

		static std::shared_ptr<spdlog::logger>& GetEngineLogger() {
			return s_engineLogger;
		}

		static std::shared_ptr<spdlog::logger>& GetApplicationLogger() {
			return s_applicationLogger;
		}

	private:

		static std::shared_ptr<spdlog::logger> s_engineLogger;
		static std::shared_ptr<spdlog::logger> s_applicationLogger;
	};
}