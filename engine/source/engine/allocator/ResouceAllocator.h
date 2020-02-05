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
#include "engine/interface/IRunTimeModule.h"

#include <map>
#include <memory>

namespace gswy {

	template<typename T>
	class ENGINE_API ResourceAllocator : public IRunTimeModule
	{
	public:
		ResourceAllocator<T>() : m_currentId(-1) {};
		ResourceAllocator<T>(const ResourceAllocator<T>&) = delete;
		ResourceAllocator<T> operator=(const ResourceAllocator<T>&) = delete;

		static ResourceAllocator<T>* GetInstance()
		{
			static ResourceAllocator<T> inst;
			return &inst;
		}


		virtual void Init() override {};
		virtual void Update(double deltaTime) override {};
		virtual void Shutdown() override {};


		T* Add(std::string filePath, std::string name)
		{
			auto it = m_resources.find(filePath);
			if (it != m_resources.end())
			{
				return it->second.second.get();
			}

			std::shared_ptr<T> resource = std::make_shared<T>();
			if (!resource->LoadFromFile(filePath))
			{
				// TODO : Engine exception
				return nullptr;
			}

			m_resources.insert(std::make_pair(name, std::make_pair(++m_currentId, resource)));

			return resource.get();
		}

		void Remove(int id)
		{
			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if (it->second.first == id)
				{
					m_resources.erase(it->first);
				}
			}
		}

		void Remove(std::string name)
		{
			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if (it->first.compare(name) == 0)
				{
					m_resources.erase(it->first);
				}
			}
		}

		T* Get(int id)
		{
			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if (it->second.first == id)
				{
					return it->second.second.get();
				}
			}

			return nullptr;
		}

		T* Get(std::string name)
		{
			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if (it->first.compare(name) == 0)
				{
					return it->second.second.get();
				}
			}

			return nullptr;
		}

		bool Has(int id)
		{
			return (Get(id) != nullptr);
		}

		bool Has(std::string name)
		{
			return (Get(name) != nullptr);
		}

	private:
		int m_currentId;
		std::map<std::string, std::pair<int, std::shared_ptr<T>>> m_resources;
	};
}
