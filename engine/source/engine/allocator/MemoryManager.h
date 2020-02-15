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
#include "engine/EngineCore.h"
#include <memory>
#include "engine/interface/IRunTimeModule.h"
#include "Allocator.h"
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
			DEBUG_PRINT("New : " + Str(typeid(T).name()) + " " + Str(sizeof(T)) + " " + Str(m_AllocatedSize));
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
			DEBUG_PRINT("Delete : " + Str(typeid(T).name()) + " " + Str(sizeof(T)) + " " + Str(*(reinterpret_cast<uint32_t*>(p) - 1)) + " " + Str(m_AllocatedSize));
			p->~T();
			Free(p, sizeof(T));
#else
			DEBUG_PRINT("Delete : " + Str(typeid(T).name()) + " " + Str(sizeof(T)));
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
			MemoryManager::Make_shared<T>(parameters...);
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
    private:
        static Allocator* LookUpAllocator(size_t size) noexcept;
    };
}

namespace gswy {

    #ifndef MyVector
    #define MyVector(T) std::vector<T, gswy::allocator<T>>
    #endif

    template <typename T>
    class allocator
    {
    public:
        
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef unsigned size_type;
        typedef unsigned difference_type;
        
        template <typename U> struct rebind { typedef std::allocator<U> other;};
        
        pointer allocate(unsigned n)
        {
        return
            reinterpret_cast<T *>
            (g_pMemoryManager->Allocate(sizeof(T) * n));
        }

        void deallocate(pointer p, unsigned n)
        {
        g_pMemoryManager->Free(p, sizeof(T) * n);
        }

        void construct(pointer p, const_reference clone)
        {
        new (p) T(clone);
        }

        void destroy(pointer p)
        {
        p->~T();
        }

        pointer address(reference x) const
        {
        return &x;
        }

        const_pointer address(const_reference x) const
        {
        return &x;
        }

        bool operator==(const allocator &rhs)
        {
            return true;
        }

        bool operator!=(const allocator &rhs)
        {
        return !operator==(rhs);
        }
    };
}