#include "SupportSkill.h"
#include "skill-system/primitives/SkillTag.h"

namespace gswy
{
	
	SupportSkill::SupportSkill()
	{
		AddSkillTag(SkillTag::SUPPORT);
	}

	SupportSkill::~SupportSkill()
	{
		RemoveSkillTag(SkillTag::SUPPORT);
	}

}