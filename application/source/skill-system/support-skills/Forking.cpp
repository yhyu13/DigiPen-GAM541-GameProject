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

	const int& Forking::GetCount()
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