/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Fork.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

#include "Fork.h"

namespace gswy
{
	Fork::Fork(): m_forkCount(0)
	{
		AddSkillTag(SkillTag::FORK);
	}

	Fork::~Fork()
	{
	}

	const int Fork::GetForkCount()
	{
		return m_forkCount;
	}

	void Fork::SetForkCount(int forkCount)
	{
		m_forkCount = forkCount;
	}
}