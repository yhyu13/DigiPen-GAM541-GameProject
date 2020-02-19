/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: InstrumentorCore.h
Purpose			: Core utilities used for instrumenting profiling.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 16/02/2020
- End Header ----------------------------*/

#pragma once

#include "Instrumentor.h"
#include "InstumentingTimer.h"


#ifdef _DEBUG
	#define TIME(name) InstrumentingTimer timer##__LINE__(name, gswy::Instrumentor::GetInstance())
#else
	#define TIME(name) InstrumentingTimer timer##__LINE__(name, gswy::Instrumentor::GetInstance())
#endif
