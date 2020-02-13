/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BitMaskSignature.h
Purpose			: Contains definitions for the bit-mask signatures.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "BitMaskSignature.h"

namespace gswy {

	bool BitMaskSignature::IsNewMatch(BitMaskSignature oldMask, BitMaskSignature systemSignature) {
		return IsAMatch(systemSignature) && !oldMask.IsAMatch(systemSignature);
	}

	bool BitMaskSignature::IsNoLongerMatched(BitMaskSignature oldMask, BitMaskSignature systemSignature) {
		return oldMask.IsAMatch(systemSignature) && !IsAMatch(systemSignature);
	}

	bool BitMaskSignature::IsAMatch(BitMaskSignature systemSignature) {
		return ((m_mask & systemSignature.m_mask) == systemSignature.m_mask);
	}
}