#pragma once

#include "primitives/SkillTag.h"

#include <set>


namespace gswy
{
	class BaseSkill {

	public:

		BaseSkill();
		virtual ~BaseSkill();

	protected:

		void AddSkillTag(const SkillTag& tag);
		void RemoveSkillTag(const SkillTag& tag);

		const int& GetId();

	private:
		std::set<SkillTag> m_tags;
		static int m_id;
	};
}
