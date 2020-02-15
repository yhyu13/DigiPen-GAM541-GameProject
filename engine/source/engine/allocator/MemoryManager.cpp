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
#include "engine-precompiled-header.h"
#include "MemoryManager.h"

#ifndef ALIGN
#define ALIGN(x, a)         (((x) + ((a) - 1)) & ~((a) - 1))
#endif

using namespace gswy;

namespace gswy {

    const uint32_t MemoryManager::kBlockSizes[] = {
        // 4-increments
        4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
        52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 

        // 32-increments
        128, 160, 192, 224, 256, 288, 320, 352, 384, 
        416, 448, 480, 512, 544, 576, 608, 640, 

        // 64-increments
        704, 768, 832, 896, 960, 1024, 
		
		// Large
		2048, 3072, 4096, 5120, 6144, 7168, 8192
    };

    const uint32_t MemoryManager::kPageSize  = 8192*4;
    const uint32_t MemoryManager::kAlignment = 4;

    // number of elements in the block size array
    const uint32_t MemoryManager::kNumBlockSizes = 
        sizeof(kBlockSizes) / sizeof(kBlockSizes[0]);

    // largest valid block size (must not be greater than 2^16)
    const uint32_t MemoryManager::kMaxBlockSize = 
        kBlockSizes[kNumBlockSizes - 1];

    size_t*        MemoryManager::m_pBlockSizeLookup;
    Allocator*     MemoryManager::m_pAllocators;
	size_t		   MemoryManager::m_AllocatedSize = 0;
}

void gswy::MemoryManager::Init()
{
    // one-time initialization
    static bool s_bInitialized = false;
    if (!s_bInitialized) {
        // initialize block size lookup table
        m_pBlockSizeLookup = new size_t[kMaxBlockSize + 1];
        size_t j = 0;
        for (size_t i = 0; i <= kMaxBlockSize; i++) {
            if (i > kBlockSizes[j]) ++j;
            m_pBlockSizeLookup[i] = j;
        }

        // Throwing exception in noexcept would terminate the program, 
        // But since it's an exception in the memory allocator, it's find to kill the program anyway.
		if (j != kNumBlockSizes - 1)
		{
			std::string msg("Memory manger pool initialization has failed.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, gswy::str2wstr(msg));
		}

        // initialize the allocators
        m_pAllocators = new Allocator[kNumBlockSizes];
        for (size_t i = 0; i < kNumBlockSizes; i++) {
            m_pAllocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
        }
        s_bInitialized = true;
    }
}

void gswy::MemoryManager::Shutdown()
{
	/* We found that shared pointers could call deleter in the very end of the exit
	   even after Shutdown() while m_pAllocators and m_pBlockSizeLookup 
	   are already deleted. To avoid that error, simply leave these memory
	   for the OS to clean up.
    delete[] m_pAllocators;
    delete[] m_pBlockSizeLookup;*/
}

void gswy::MemoryManager::Update(double deltaTime)
{
}

Allocator* gswy::MemoryManager::LookUpAllocator(size_t size) noexcept
{
    // check eligibility for lookup
    return (size <= kMaxBlockSize) ? m_pAllocators + m_pBlockSizeLookup[size] : nullptr;
}

void* gswy::MemoryManager::Allocate(size_t size, size_t alignment) noexcept
{
    uint8_t* p;
    size += alignment;
    if (Allocator* pAlloc = LookUpAllocator(size))
        p = reinterpret_cast<uint8_t*>(pAlloc->Allocate());
    else
        p = reinterpret_cast<uint8_t*>(malloc(size));

    p = reinterpret_cast<uint8_t*>(ALIGN(reinterpret_cast<size_t>(p), alignment));
    
    return reinterpret_cast<void*>(p);
}

// Replacement for malloc()

void* gswy::MemoryManager::Allocate(size_t size) noexcept
{
	/*return malloc(size);*/
#if CUSTOM_ALLOCATOR 
	if (size <= kMaxBlockSize)
	{
		return (m_pAllocators + m_pBlockSizeLookup[size])->Allocate();
	}
	else
	{
		return malloc(size);
	}
#else
	return malloc(size);
#endif // CUSTOM_ALLOCATOR 
}

// Replacement for free()

void gswy::MemoryManager::Free(void* p, size_t size) noexcept
{
	// Original
	//   (size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[size])->Free(p) 
	//       : free(p);
#if CUSTOM_ALLOCATOR 
	// Storing m_szDataSize as uint32_t right before each block
	if (size <= kMaxBlockSize)
	{
		(m_pAllocators + m_pBlockSizeLookup[*(reinterpret_cast<uint32_t*>(p) - 1)])->Free(p);
	}
	else
	{
		free(p);
	}
#else
	free(p);
#endif // CUSTOM_ALLOCATOR 
}

