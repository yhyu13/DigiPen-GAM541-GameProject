/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: LoggingCore.h
Purpose			: Contains easy-to-use logging macro definitions.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/10/2020
- End Header ----------------------------*/

#pragma once

#include "engine/logging/Logger.h"

using namespace gswy;

//////// ENGINE LOGGING ////////
#define ENGINE_TRACE(...)			Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)			Logger::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_DEBUG(...)			Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define ENGINE_WARN(...)			Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)			Logger::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...)		Logger::GetEngineLogger()->critical(__VA_ARGS__)


//////// APPLICATION LOGGING ////////
#define APP_TRACE(...)		Logger::GetApplicationLogger()->trace(__VA_ARGS__)
#define APP_INFO(...)		Logger::GetApplicationLogger()->info(__VA_ARGS__)
#define APP_DEBUG(...)		Logger::GetApplicationLogger()->debug(__VA_ARGS__)
#define APP_WARN(...)		Logger::GetApplicationLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)		Logger::GetApplicationLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...)	Logger::GetApplicationLogger()->critical(__VA_ARGS__)