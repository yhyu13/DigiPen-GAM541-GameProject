/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/14/2020
- End Header ----------------------------*/
#pragma once
#include <memory>
#include "Allocator.h"
#include "engine/EngineCore.h"
#include "engine/interface/IRunTimeModule.h"
#include "engine/exception/EngineException.h"

#define CUSTOM_ALLOCATOR 1

namespace gswy {

    class MemoryManager : public IRunTimeModule
    {
    public:
		static MemoryManager* GetInstance()
		{
			static MemoryManager inst;
			return &inst;
		}

        // Replacement for new 
        template<class T, typename... Arguments>
        static T* New(Arguments... parameters) noexcept
        {
			m_AllocatedSize += sizeof(T);
			PRINT("New : " + Str(typeid(T).name()) + " " + Str(sizeof(T)) + " " + Str(m_AllocatedSize));
#if CUSTOM_ALLOCATOR 
			return new (Allocate(sizeof(T))) T(parameters...);
#else
			return new T(parameters...);
#endif // CUSTOM_ALLOCATOR    
        }

        // Replacement for delete
        template<class T>
        static void Delete(T* p) noexcept
        {
			m_AllocatedSize -= sizeof(T);
#if CUSTOM_ALLOCATOR 
			PRINT("Delete : " + Str(typeid(T).name()) + " " + Str(sizeof(T)) + " " + Str((reinterpret_cast<BlockHeader*>(p) - 1)->size) + " " + Str(m_AllocatedSize));
			p->~T();
			Free(p, sizeof(T));
#else
			PRINT("Delete : " + Str(typeid(T).name()) + " " + Str(sizeof(T)) + " " + Str(m_AllocatedSize));
			delete p;
#endif // CUSTOM_ALLOCATOR 
           
        }

		// Replacement for make_shared
		template<class T, typename... Arguments>
		static std::shared_ptr<T> Make_shared(Arguments... parameters) noexcept
		{
#if CUSTOM_ALLOCATOR 
			return std::shared_ptr<T>(New<T>(parameters...), Delete<T>);
#else
			return std::make_shared<T>(parameters...);
#endif // CUSTOM_ALLOCATOR 
		}

		static size_t GetCurrentManagedMemory()
		{
			return m_AllocatedSize;
		}

    public:
        virtual ~MemoryManager() noexcept {};

		virtual void Init() override;
		virtual void Update(double deltaTime) override;
		virtual void Shutdown() override;

        static void* Allocate(size_t size, size_t alignment) noexcept;

        // Replacement for malloc()
		static void* Allocate(size_t size) noexcept;
		
		// Replacement for free()
		static void  Free(void* p, size_t size) noexcept;
		
    private:
        static const uint32_t kBlockSizes[];

        static const uint32_t kPageSize;
        static const uint32_t kAlignment;

        // number of elements in the block size array
        static const uint32_t kNumBlockSizes;

        // largest valid block size
        static const uint32_t kMaxBlockSize;

        static size_t*        m_pBlockSizeLookup;
        static Allocator*     m_pAllocators;
		static size_t		  m_AllocatedSize;

		template<typename T>
		friend struct Mallocator;

    private:
        static Allocator* LookUpAllocator(size_t size) noexcept;
    };
}

namespace gswy {

    #ifndef MyVector
    #define MyVector(T) std::vector<T, gswy::Mallocator<T>>
    #endif

	/*
		Reference https://en.cppreference.com/w/cpp/named_req/Allocator
	*/
	template <class T>
	struct Mallocator
	{
		typedef T value_type;

		Mallocator()
		{
			MemoryManager::GetInstance();
		}
		template <class U> constexpr Mallocator(const Mallocator <U>&) noexcept {}

		[[nodiscard]] T* allocate(std::size_t n) {
			if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
				throw std::bad_alloc();

			if (auto p = static_cast<T*>(MemoryManager::Allocate(n * sizeof(T))))
			{
				MemoryManager::m_AllocatedSize += n * sizeof(T);
				PRINT("Allocate vector : " + Str(typeid(T).name()) + " " + Str(n) + " " + Str(sizeof(T)) + " " + Str(n * sizeof(T)) + " " + Str(MemoryManager::m_AllocatedSize));
				return p;
			}
			throw std::bad_alloc();
		}
		void deallocate(T* p, std::size_t n) noexcept 
		{ 
			MemoryManager::Free(p, sizeof(T) * n); 
			MemoryManager::m_AllocatedSize -= n * sizeof(T);
			PRINT("Free vector : " + Str(typeid(T).name()) + " " + Str(n) + " " + Str(sizeof(T)) + " " + Str(n * sizeof(T)) + " " + Str(MemoryManager::m_AllocatedSize));
		}
	};
}

template <class T, class U>
bool operator==(const gswy::Mallocator <T>&, const gswy::Mallocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const gswy::Mallocator <T>&, const gswy::Mallocator <U>&) { return false; }