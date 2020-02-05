/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BaseComponent.h
Purpose			: Contains declarations for component struct.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#pragma once

namespace gswy {

	/*
		Each component-type gets a type-index. The type-index is used in calculating
		component-signatures (bit-masks).

		For more information, please check BitMaskSignature.h.
	*/
	struct ComponentTypeIndex {
		static unsigned int s_index;
	};


	/*
		Every component in the ECS architecture is simply a data-storage unit.

		To create new components:

		struct Transform : BaseComponent<Transform> {
			Position(float x, float y) : x(x), y(y) {};
			float x;
			float y;
		};
	*/
	template <typename ComponentType>
	struct BaseComponent {

		/*
			All components of a given type belong to the same type-index. For example,
			All Position components have the same type-index.
		*/
		static inline const unsigned int TypeIndex() {
			static unsigned int index = ComponentTypeIndex::s_index++;
			return index;
		}
	};

	/*
		Use this function to get the family-code for a component-type.
		Example usage: GetComponentFamily<Transform>()
	*/
	template <typename ComponentType>
	static unsigned int GetComponentTypeIndex() {
		return BaseComponent<ComponentType>::TypeIndex();
	}
}


