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
#pragma once
#include <atomic>
#include "engine/EngineCore.h"
namespace gswy
{
	struct atomic_lock_guard {
		atomic_lock_guard() = delete;
		atomic_lock_guard(atomic_lock_guard&&) = delete;
		atomic_lock_guard(const atomic_lock_guard&) = delete;
		atomic_lock_guard& operator=(const atomic_lock_guard&) = delete;
		explicit atomic_lock_guard(std::atomic_flag& flag);
		~atomic_lock_guard();
	private:
		std::atomic_flag* m_lock;
	};

	class BaseClassAtomicFlag
	{
	public:
		NONCOPYABLE(BaseClassAtomicFlag);

#define LOCK_GUARD() atomic_lock_guard lock(m_flag);

		BaseClassAtomicFlag()
		{
			m_flag.clear();
		}

	protected:
		std::atomic_flag m_flag;
	};
}