#include "Fork.h"

namespace gswy
{
	Fork::Fork(): m_forkCount(0), m_fork(true)
	{
		AddSkillTag(SkillTag::FORK);
	}

	Fork::~Fork()
	{

	}

	const int& Fork::GetForkCount()
	{
		return m_forkCount;
	}

	void Fork::SetForkCount(const int& forkCount)
	{
		m_forkCount = forkCount;
	}

	const bool& Fork::IsFork()
	{
		return m_fork;
	}

	void Fork::SetFork(const bool& fork)
	{
		m_fork = fork;
	}
}