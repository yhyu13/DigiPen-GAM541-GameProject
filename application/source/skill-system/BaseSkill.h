/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseSkill.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/13/2020
- End Header ----------------------------*/

#pragma once

#include "primitives/SkillTag.h"

#include <set>
#include <string>


namespace gswy
{
	class BaseSkill {

	public:

		BaseSkill();
		virtual ~BaseSkill();

		std::set<SkillTag> GetSkillTags();

		std::string& GetIcon();
		void SetIcon(std::string& icon);

		std::string& GetIconGray();
		void SetIconGray(std::string& icon);

	protected:

		virtual void AddSkillTag(const SkillTag& tag);
		virtual void RemoveSkillTag(const SkillTag& tag);

		const int& GetId();

	private:
		std::string m_icon;
		std::string m_iconGray;
		std::set<SkillTag> m_tags;
		static int m_id;
	};
}
