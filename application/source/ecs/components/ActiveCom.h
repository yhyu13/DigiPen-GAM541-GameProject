/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/12/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"

namespace gswy
{
	/* Data class of LIfe Time */
	struct ActiveCom : BaseComponent<ActiveCom> {

		ActiveCom()
		{
			m_active = true;
		}

		explicit ActiveCom(bool active)
		{
			m_active = active;
		}

		void SetActive(bool active)
		{
			m_active = active;
		}

		bool IsActive()
		{
			return m_active;
		}

	private:
		bool m_active;
	};
}
