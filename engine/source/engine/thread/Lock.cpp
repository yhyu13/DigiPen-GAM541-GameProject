/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/21/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Lock.h"

gswy::atomic_lock_guard::atomic_lock_guard(std::atomic_flag& flag)
{
	m_lock = &flag;
	while (m_lock->test_and_set(std::memory_order_acquire)) {}
}

gswy::atomic_lock_guard::~atomic_lock_guard()
{
	m_lock->clear(std::memory_order_release);
}
