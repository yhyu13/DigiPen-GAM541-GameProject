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
#include <stddef.h>
#include <stdint.h>
#include <mutex>

#define THREAD_SAFE 0

namespace gswy {

    struct BlockHeader {
        // union-ed with data
        BlockHeader* pNext;
    };

    struct PageHeader {
        PageHeader* pNext;
        BlockHeader* Blocks() {
			// Original:
			//return reinterpret_cast<BlockHeader*>(this + 1);

			// Storing m_szBlockSize as uint32_t right before each block
            return reinterpret_cast<BlockHeader*>(reinterpret_cast<uint8_t*>(this + 1) + sizeof(uint32_t));
        }
    };

    class Allocator {
        public:
                // debug patterns
                static const uint8_t PATTERN_ALIGN = 0xFC;
                static const uint8_t PATTERN_ALLOC = 0xFD;
                static const uint8_t PATTERN_FREE  = 0xFE;

                Allocator() noexcept;
                explicit Allocator(size_t data_size, size_t page_size, size_t alignment) noexcept;
                ~Allocator() noexcept;

                // disable copy & assignment
                Allocator(const Allocator& clone) = delete;
                Allocator &operator=(const Allocator &rhs) = delete;

                // resets the allocator to a new configuration
                void Reset(size_t data_size, size_t page_size, size_t alignment) noexcept;

                // alloc and free blocks
                void* Allocate() noexcept;
                void  Free(void* p) noexcept;
                void  FreeAll() noexcept;
        private:
#if defined(_DEBUG)
                // fill a free page with debug patterns
                void FillFreePage(PageHeader* pPage) noexcept;

                // fill a block with debug patterns
                void FillFreeBlock(BlockHeader* pBlock) noexcept;

                // fill an allocated block with debug patterns
                void FillAllocatedBlock(BlockHeader* pBlock) noexcept;
#endif

                // gets the next block
                BlockHeader* NextBlock(BlockHeader* pBlock) noexcept;

                // the page list
                PageHeader* m_pPageList;

                // the free block list
                BlockHeader* m_pFreeList;

                size_t      m_szDataSize;
                size_t      m_szPageSize;
                size_t      m_szAlignmentSize;
                size_t      m_szBlockSize;
                size_t      m_nBlocksPerPage;

                // statistics
                size_t    m_nPages;
                size_t    m_nBlocks;
                size_t    m_nFreeBlocks;

#if THREAD_SAFE
                std::mutex  mtx;
#endif
    };
}

