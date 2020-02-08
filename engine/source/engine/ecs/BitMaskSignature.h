/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BitMaskSignature.h
Purpose			: Contains declarations for the bit-mask signatures.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#pragma once

#include "BaseComponent.h"

namespace gswy {

	/*
		This class holds bit-masking functionality.

		These signatures (bit-masks) serves two chief purposes :

			1#	Each entity maintains its signature that helps identify what are the
				components the entity owns.

			2#	Each system maintains its signature that helps identify which components
				the system cares about, and thus which entities the system is interested in.

		For details on systems, please see BaseComponentSystem.h
	*/
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