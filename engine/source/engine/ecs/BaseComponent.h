#pragma once

/*

	To create new components:

	struct Transform : BaseComponent<Transform> {
		Position(float x, float y) : x(x), y(y) {};
		float x;
		float y;
	};

*/

namespace gswy {

	struct ComponentTypeIndex {
		static unsigned int s_index;
	};

	template <typename ComponentType>
	struct BaseComponent {

		/*
			All components of a given type belong to the same family. For example,
			All Position components have the same family-code.
		*/
		static inline const unsigned int TypeIndex() {
			static unsigned int index = ComponentTypeIndex::s_index++;
			return index;
		}
	};

	/*
		Use this function to get the family-code for a component-type.
		Call this function like this for a component-type: GetComponentFamily<Transform>()
	*/
	template <typename ComponentType>
	static unsigned int GetComponentTypeIndex() {
		return BaseComponent<ComponentType>::TypeIndex();
	}
}


