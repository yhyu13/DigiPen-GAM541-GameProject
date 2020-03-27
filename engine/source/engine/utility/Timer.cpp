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
#include "engine-precompiled-header.h"
#include "Timer.h"

using namespace std::chrono;

gswy::Timer::Timer()
{
	if (std::chrono::steady_clock::is_steady == false)
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"C++11 std::chrono::steady_clock::is_steady() returns FALSE.");
	last = steady_clock::now();
}


void gswy::Timer::Reset() noexcept
{
	last = steady_clock::now();
}

double gswy::Timer::Mark() noexcept
{
	const duration<double> Time = steady_clock::now() - last;
	return Time.count();
}