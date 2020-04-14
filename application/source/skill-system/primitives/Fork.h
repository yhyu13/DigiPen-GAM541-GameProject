/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Fork.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/15/2020
- End Header ----------------------------*/

#pragma once

#include "skill-system/BaseSkill.h"

namespace gswy
{
	class Fork : virtual public BaseSkill
	{
	public:

		Fork();
		~Fork();

		const int GetForkCount();
		void SetForkCount(int forkCount);

	private:

		int m_forkCount;
	};
}
