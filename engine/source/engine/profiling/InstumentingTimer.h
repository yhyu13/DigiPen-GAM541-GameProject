/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Timer.h
Purpose			: Contains declarations for a timer for instrumenting profiling.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/15/2020
- End Header ----------------------------*/

#pragma once

#include <chrono>

//#include "Instrumentor.h"

namespace gswy {

	class Instrumentor;

	/*
		InstrumentingTimer class follows RAII pattern.
	*/
	class InstrumentingTimer {

	public:

		InstrumentingTimer(const char* name, Instrumentor* instrumentor);
		~InstrumentingTimer();

		void End();

	protected:


	private:

		const char* m_name;
		bool m_stopped;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_timeBegin;
		Instrumentor* m_instumentor;
	};

}
