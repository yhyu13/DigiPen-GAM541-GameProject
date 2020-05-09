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

#include <set>
#include "BaseComponent.h"

#define USE_BITMASK 1

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
#if USE_BITMASK
			m_mask |= ((uint64_t)1 << GetComponentTypeIndex<ComponentType>());
#else
			m_mask.insert(GetComponentTypeIndex<ComponentType>());
#endif // USE_BITMASK
		}

		template<typename ComponentType>
		void RemoveComponent() {
#if USE_BITMASK
			m_mask &= ~((uint64_t)1 << GetComponentTypeIndex<ComponentType>());
#else
			m_mask.erase(GetComponentTypeIndex<ComponentType>());
#endif // USE_BITMASK
		}

		bool IsNewMatch(BitMaskSignature& oldMask,  BitMaskSignature& systemMask) ;
		bool IsNoLongerMatched( BitMaskSignature& oldMask,  BitMaskSignature& systemMask) ;

	private:
		bool IsAMatch( BitMaskSignature& systemMask) ;
#if USE_BITMASK
		uint64_t m_mask = 0;
#else
		std::set<unsigned int> m_mask;
#endif // USE_BITMASK
	};

}