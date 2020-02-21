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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Allocator.h"

#ifndef ALIGN
#define ALIGN(x, a)         (((x) + ((a) - 1)) & ~((a) - 1))
#endif

using namespace gswy;

gswy::Allocator::Allocator() noexcept
        : m_pPageList(nullptr), m_pFreeList(nullptr), 
        m_szDataSize(0), m_szPageSize(0), 
        m_szAlignmentSize(0), m_szBlockSize(0), m_nBlocksPerPage(0),
		m_nPages(0), m_nBlocks(0),m_nFreeBlocks(0)
{
}

gswy::Allocator::Allocator(size_t data_size, size_t page_size, size_t alignment) noexcept
        : m_pPageList(nullptr), m_pFreeList(nullptr),
		m_szDataSize(0), m_szPageSize(0),
		m_szAlignmentSize(0), m_szBlockSize(0), m_nBlocksPerPage(0),
		m_nPages(0), m_nBlocks(0), m_nFreeBlocks(0)
{
    Reset(data_size, page_size, alignment);
}

gswy::Allocator::~Allocator() noexcept
{
    FreeAll();
}

void gswy::Allocator::Reset(size_t data_size, size_t page_size, size_t alignment) noexcept
{
    FreeAll();

#if THREAD_SAFE
    // Lock
	std::lock_guard<std::mutex>lock(mtx);
#endif

    m_szDataSize = data_size;
	m_szPageSize = page_size;

    size_t minimal_size = (sizeof(BlockHeader) > m_szDataSize) ? sizeof(BlockHeader) : m_szDataSize;
    // this magic only works when alignment is 2^n, which should general be the case
    // because most CPU/GPU also requires the aligment be in 2^n
    // but still we use a assert to guarantee it
    assert(alignment > 0 && ((alignment & (alignment-1))) == 0);

    m_szBlockSize = ALIGN(minimal_size, alignment);
	
	// Storing m_szDataSize as header_t right before each block
	assert(m_szBlockSize == (size_t)header_t(m_szBlockSize));

    m_szAlignmentSize = m_szBlockSize - minimal_size;
	m_nBlocksPerPage = (m_szPageSize - sizeof(PageHeader)) / (m_szBlockSize + sizeof(BlockHeader));
}

void* gswy::Allocator::Allocate() noexcept
{

#if THREAD_SAFE
	// Lock
	std::lock_guard<std::mutex>lock(mtx);
#endif

    if (!m_pFreeList) {
        // allocate a new page
        PageHeader* pNewPage = reinterpret_cast<PageHeader*>(new uint8_t[m_szPageSize]);
		pNewPage->pNext = nullptr;
        ++m_nPages;
        m_nBlocks += m_nBlocksPerPage;
        m_nFreeBlocks += m_nBlocksPerPage;

#if defined(_DEBUG)
        FillFreePage(pNewPage);
#endif

        if (m_pPageList) {
            pNewPage->pNext = m_pPageList;
        }

        m_pPageList = pNewPage;

        BlockHeader* pBlock = pNewPage->Blocks();
		auto size = static_cast<header_t>(m_szDataSize);
        // link each block in the page
        for (uint32_t i = 1; i < m_nBlocksPerPage; i++) {
			// Storing m_szDataSize as header_t right before each block
			pBlock->size = size;
            pBlock->pNext = NextBlock(pBlock);
            pBlock = NextBlock(pBlock);
        }
		//link the last block
		pBlock->size = size;
        pBlock->pNext = nullptr;

        m_pFreeList = pNewPage->Blocks();
    }
    BlockHeader* freeBlock = m_pFreeList;
    m_pFreeList = m_pFreeList->pNext;
    --m_nFreeBlocks;

#if defined(_DEBUG)
    FillAllocatedBlock(freeBlock);
#endif

	/*
		return blockheader + 1 as the diagram below:
			 BlockHeader*  void*
			[Header block][User content]
	*/
    return reinterpret_cast<void*>(freeBlock+1);
}

void gswy::Allocator::Free(void* p) noexcept
{

#if THREAD_SAFE
	// Lock
	std::lock_guard<std::mutex>lock(mtx);
#endif

    BlockHeader* block = reinterpret_cast<BlockHeader*>(p)-1;

#if defined(_DEBUG)
    FillFreeBlock(block);
#endif

    block->pNext = m_pFreeList;
    m_pFreeList = block;
    ++m_nFreeBlocks;
}

void gswy::Allocator::FreeAll() noexcept
{

#if THREAD_SAFE
	// Lock
	std::lock_guard<std::mutex>lock(mtx);
#endif

    PageHeader* pPage = m_pPageList;
	uint8_t* _p = reinterpret_cast<uint8_t*>(pPage);
    while(pPage) {
        _p = reinterpret_cast<uint8_t*>(pPage);
        pPage = pPage->pNext;
        delete[] _p;
    }

    m_pPageList = nullptr;
    m_pFreeList = nullptr;

    m_nPages        = 0;
    m_nBlocks       = 0;
    m_nFreeBlocks   = 0;
}

#if defined(_DEBUG)
void gswy::Allocator::FillFreePage(PageHeader *pPage) noexcept
{
    // page header
    pPage->pNext = nullptr;
 
    // blocks
    BlockHeader *pBlock = pPage->Blocks();
    for (uint32_t i = 0; i < m_nBlocksPerPage; i++)
    {
        FillFreeBlock(pBlock);
        pBlock = NextBlock(pBlock);
    }
}
 
void gswy::Allocator::FillFreeBlock(BlockHeader *pBlock) noexcept
{
    // block header + data
    memset(pBlock+1, PATTERN_FREE, m_szBlockSize - m_szAlignmentSize);
 
    // alignment
    memset(reinterpret_cast<uint8_t*>(pBlock+1) + m_szBlockSize - m_szAlignmentSize, 
                PATTERN_ALIGN, m_szAlignmentSize);
}
 
void gswy::Allocator::FillAllocatedBlock(BlockHeader *pBlock) noexcept
{
    // block header + data
    memset(pBlock+1, PATTERN_ALLOC, m_szBlockSize - m_szAlignmentSize);
 
    // alignment
    memset(reinterpret_cast<uint8_t*>(pBlock+1) + m_szBlockSize - m_szAlignmentSize, 
                PATTERN_ALIGN, m_szAlignmentSize);
}
 
#endif

gswy::BlockHeader* gswy::Allocator::NextBlock(BlockHeader *pBlock) noexcept
{
	// Original:
    return reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t*>(pBlock+1) + m_szBlockSize);
}