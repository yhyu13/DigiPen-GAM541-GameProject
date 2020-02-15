/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"
#include "engine/exception/EngineException.h"

namespace gswy
{
	/*
	Data class that stores references to the current animation and maintain an animation state map
	*/
	struct AnimationCom : BaseComponent<AnimationCom> {

		void Add(std::string name, std::string stateName)
		{
			auto animation = ResourceAllocator<Animation>::GetInstance()->Get(name);
			m_animationStateMap[stateName] = MemoryManager::Make_shared<Animation>(*animation);
		}
		std::shared_ptr<Animation> GetCurrentAnimation()
		{
			if (auto temp = m_animation.lock())
			{
				return temp;
			}
			else
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Animation state " + str2wstr(m_animationState) + L"has failed to be getted!");
			}
		}
		std::string GetCurrentAnimationState()
		{
			return m_animationState;
		}
		void SetCurrentAnimationState(const std::string state)
		{
			if (m_animationState.compare(state) == 0)
			{
				return;
			}
			if (m_animationStateMap.find(state) != m_animationStateMap.end())
			{
				m_animationState = state;
				m_animation = m_animationStateMap[state];
				if (auto temp = m_animation.lock())
				{
					temp->Reset();
				}
				else
				{
					// TODO : Engine exception
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Animation state " + str2wstr(m_animationState) + L"has failed to be set!");
				}
			}
			else
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Animation state " + str2wstr(m_animationState) + L"has not been managed!");
			}
		}
	private:
		std::weak_ptr<Animation> m_animation;
		std::string m_animationState;
		std::map<std::string, std::shared_ptr<Animation>> m_animationStateMap;
	};
}
