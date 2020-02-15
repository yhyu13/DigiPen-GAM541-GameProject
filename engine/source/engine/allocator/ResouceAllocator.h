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
#include "engine/exception/EngineException.h"
#include "engine/EngineCore.h"

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

		/*
			Call T::Create(std::string filePath) internally,
			meaning T must have a Create() function
		*/
		std::shared_ptr<T> Create(std::string filePath, std::string name)
		{
			auto it = m_resources.find(filePath);
			if (it != m_resources.end())
			{
				return it->second.second;
			}
			std::shared_ptr<T> resource = T::Create(filePath);
			if (!resource)
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Resource at " + str2wstr(filePath) + L" has failed to load!");
			}
			m_resources.insert(std::make_pair(name, std::make_pair(++m_currentId, resource)));
			return resource;
		}

		/*
			Call (T*)->LoadFromFile(std::string filePath) internally,
			meaning T* must have a LoadFromFile() function
		*/
		std::shared_ptr<T> LoadFromFile(std::string filePath, std::string name)
		{
			auto it = m_resources.find(filePath);
			if (it != m_resources.end())
			{
				return it->second.second;
			}
			std::shared_ptr<T> resource = MemoryManager::Make_shared<T>();
			if (!resource->LoadFromFile(filePath))
			{
				// TODO : Engine exception
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Resource at " + str2wstr(filePath) + L" has failed to load!");
			}
			m_resources.insert(std::make_pair(name, std::make_pair(++m_currentId, resource)));
			return resource;
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
			auto it = m_resources.find(name);
			if (it != m_resources.end())
			{
				m_resources.erase(it->first);
			}
		}

		std::shared_ptr<T> Get(int id)
		{
			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if (it->second.first == id)
				{
					return it->second.second;
				}
			}
			// TODO : Engine exception
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Resource with id " + str2wstr(Str(id)) + L" is not managed!");
		}

		std::shared_ptr<T> Get(std::string name)
		{
			auto it = m_resources.find(name);
			if (it != m_resources.end())
			{
				return it->second.second;
			}
			// TODO : Engine exception
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Resource with id " + str2wstr(name) + L" is not managed!");
		}

		// Deprecated
		//bool Has(int id)
		//{
		//	return (Get(id) != nullptr);
		//}
		// Deprecated
		//bool Has(std::string name)
		//{
		//	return (Get(name) != nullptr);
		//}

	private:
		int m_currentId;
		std::map<std::string, std::pair<int, std::shared_ptr<T>>> m_resources;
	};
}
