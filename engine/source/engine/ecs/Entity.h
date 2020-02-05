#pragma once

#include "engine/EngineCore.h"

namespace gswy {

	struct ENGINE_API Entity {

		unsigned int m_id;

		Entity(unsigned int id) {
			m_id = id;
		}

		Entity() = default;

		Entity(const Entity& other) : m_id(other.m_id) {
		}

		friend inline bool operator==(const Entity& lhs, const Entity& rhs) {
			return lhs.m_id == rhs.m_id;
		}

		friend inline bool operator<(const Entity& lhs, const Entity& rhs) {
			return lhs.m_id < rhs.m_id;
		}
	};

}