/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/25/2020
- End Header ----------------------------*/
#pragma once
#include <chrono>
#include "engine/exception/EngineException.h"

namespace gswy
{
	class Timer
	{
	public:
		Timer();

		void Reset() noexcept;
		double Mark() noexcept;
	private:
		std::chrono::steady_clock::time_point last;
	};
}