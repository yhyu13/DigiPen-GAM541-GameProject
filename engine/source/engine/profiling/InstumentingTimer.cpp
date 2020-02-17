/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Timer.cpp
Purpose			: Contains definitions for a timer for instrumenting profiling.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/15/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "InstumentingTimer.h"
#include "engine/EngineCore.h"
#include "Instrumentor.h"

namespace gswy {


	InstrumentingTimer::InstrumentingTimer(const char* name, Instrumentor* instrumentor) :m_name(name), m_stopped(false), m_instumentor(instrumentor) {
		m_timeBegin = std::chrono::high_resolution_clock::now();
	}

	InstrumentingTimer::~InstrumentingTimer() {
		if (!m_stopped) {
			End();
		}
	}

	void InstrumentingTimer::End() {
		decltype(auto) timeEnd = std::chrono::high_resolution_clock::now();

		decltype(auto) begin = std::chrono::time_point_cast<std::chrono::microseconds>(m_timeBegin).time_since_epoch().count();
		decltype(auto) end = std::chrono::time_point_cast<std::chrono::microseconds>(timeEnd).time_since_epoch().count();
		decltype(auto) duration = end - begin;
		decltype(auto) timeInMilliSeconds = duration * 0.001;

		m_stopped = true;

		m_instumentor->AddInstrumentorResult({m_name, timeInMilliSeconds, "ms"});
	}

}