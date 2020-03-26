#pragma once

#include "skill-system/BaseSkill.h"

namespace gswy
{
	class Fork : virtual public BaseSkill
	{
	public:

		Fork();
		~Fork();

		const int& GetForkCount();
		void SetForkCount(const int& forkCount);

		const bool& IsFork();
		void SetFork(const bool& fork);

	private:

		int m_forkCount;
		bool m_fork;
	};
}
