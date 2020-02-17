/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Instrumentor.h
Purpose			: Contains declarations for a instrumenting profiling manager.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/16/2020
- End Header ----------------------------*/

#pragma once

#include "engine-precompiled-header.h"

namespace gswy {

	struct InstrumentorResult {
		const char* m_name;
		double m_time;
		const char* m_timeUnit;

		InstrumentorResult() = default;
		~InstrumentorResult() = default;
	};

	class Instrumentor {

	public:

		static Instrumentor* GetInstance();
		~Instrumentor();

		void BeginSession();
		void EndSession();
		void AddInstrumentorResult(const InstrumentorResult& result);
		std::map<const char*, InstrumentorResult>& GetResults();

	private:

		Instrumentor();

		std::map<const char*, InstrumentorResult> m_results;
	};
}