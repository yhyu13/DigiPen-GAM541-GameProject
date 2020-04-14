/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 04/13/2020
- End Header ----------------------------*/

#include "Damage.h"

gswy::Damage::Damage()
	:
	m_baseDmanage(0.f),
	m_multiplier(1.0f)
{
}

gswy::Damage::~Damage()
{
}

void gswy::Damage::SetBaseDamange(float baseDmanage)
{
	m_baseDmanage = baseDmanage;
}

const float gswy::Damage::GetDamage()
{
	return m_baseDmanage * m_multiplier;
}

void gswy::Damage::SetDamangeMultipler(float multiplier)
{
	if (multiplier > 0)
	{
		m_multiplier = multiplier;
	}
}

void gswy::Damage::ApplyDamangeMultipler(float multiplier)
{
	if (multiplier > 0)
	{
		m_multiplier *= multiplier;
	}
}
