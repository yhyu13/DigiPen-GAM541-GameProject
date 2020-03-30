/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/30/2020
- End Header ----------------------------*/

#pragma once

#include "SupportSkill.h"

namespace gswy
{
	class IncreaseAOE : public SupportSkill
	{
	public:

		IncreaseAOE();
		~IncreaseAOE();

		const float& GetValue();
		void SetValue(const float value);
		virtual void HandleSkill(BaseSkill* skill) override;
		virtual void RemoveSkill(std::shared_ptr<BaseSkill> skill) override;

	private:

		float m_value;
	};
}
