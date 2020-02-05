#pragma once

#include "BaseComponent.h"

namespace gswy {


	class BitMaskSignature {

	public:
		template<typename ComponentType>
		void AddComponent() {
			m_mask |= (1 << GetComponentTypeIndex<ComponentType>());
		}

		template<typename ComponentType>
		void RemoveComponent() {
			m_mask &= ~(1 << GetComponentTypeIndex<ComponentType>());
		}

		bool IsNewMatch(BitMaskSignature oldMask, BitMaskSignature systemMask);
		bool IsNoLongerMatched(BitMaskSignature oldMask, BitMaskSignature systemMask);
		bool IsAMatch(BitMaskSignature systemMask);

	private:
		unsigned int m_mask = 0;

	};

}