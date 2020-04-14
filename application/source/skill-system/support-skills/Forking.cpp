/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Forking.cpp
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

#include "Forking.h"
#include "../primitives/Projectile.h"
#include "../primitives/Fork.h"

namespace gswy
{

	Forking::Forking(): SupportSkill(SupportSkillType::FORK), m_count(2)
	{
	}

	Forking::~Forking()
	{
	}

	const int Forking::GetCount()
	{
		return m_count;
	}

	void Forking::SetCount(const int count)
	{
		m_count = count;
	}

	void Forking::HandleSkill(BaseSkill* skill)
	{
		Fork* fork = dynamic_cast<Fork*> (skill);
		if (fork != nullptr)
		{
			fork->SetForkCount(m_count);
		}
	}

	void Forking::RemoveSkill(std::shared_ptr<BaseSkill> skill)
	{
		std::shared_ptr<Fork> fork = std::dynamic_pointer_cast<Fork> (skill);
		if (fork != nullptr)
		{
			fork->SetForkCount(0);
		}
	}
}