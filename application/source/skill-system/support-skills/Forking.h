/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Forking.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

#pragma once

#include "SupportSkill.h"

namespace gswy
{
	class Forking : public SupportSkill
	{
	public:

		Forking();
		~Forking();

		const int GetCount();
		void SetCount(const int count);
		virtual void HandleSkill(BaseSkill* skill) override;
		virtual void RemoveSkill(std::shared_ptr<BaseSkill> skill) override;

	private:

		int m_count;
	};
}
