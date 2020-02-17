/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Instrumentor.cpp
Purpose			: Contains definitions for a instrumenting profiling manager.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/16/2020
- End Header ----------------------------*/


#include "engine-precompiled-header.h"
#include "Instrumentor.h"

namespace gswy {

	Instrumentor::Instrumentor() {
	}

	Instrumentor::~Instrumentor() {
	}

	Instrumentor* Instrumentor::GetInstance() {
		static Instrumentor instance;
		return &instance;
	}

	void Instrumentor::BeginSession() {
	}

	void Instrumentor::EndSession() {
	}

	void Instrumentor::AddInstrumentorResult(const InstrumentorResult& result) {
		auto itr = m_results.find(result.m_name);
		if (itr != m_results.end()) {
			(*itr).second.m_time = result.m_time;
		}
		else {
			m_results.insert({ result.m_name , result });
		}
	}

	std::map<const char*, InstrumentorResult>& Instrumentor::GetResults() {
		return m_results;
	}
}