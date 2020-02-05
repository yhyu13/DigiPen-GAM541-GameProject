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