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
	m_flag.clear();
}

gswy::Allocator::Allocator(size_t data_size, size_t page_size, size_t alignment) noexcept
        : m_pPageList(nullptr), m_pFreeList(nullptr),
		m_szDataSize(0), m_szPageSize(0),
		m_szAlignmentSize(0), m_szBlockSize(0), m_nBlocksPerPage(0),
		m_nPages(0), m_nBlocks(0), m_nFreeBlocks(0)
{
	m_flag.clear();
    Reset(data_size, page_size, alignment);
}

gswy::Allocator::~Allocator() noexcept
{
    FreeAll();
}

void gswy::Allocator::Reset(size_t data_size, size_t page_size, size_t alignment) noexcept
{
    FreeAll();

	while (m_flag.test_and_set(std::memory_order_acquire)) {}

    m_szDataSize = data_size;
	m_szPageSize = page_size;

    size_t minimal_size = (sizeof(BlockHeader) > m_szDataSize) ? sizeof(BlockHeader) : m_szDataSize;
    // this magic only works when alignment is 2^n, which should general be the case
    // because most CPU/GPU also requires the aligment be in 2^n
    // but still we use a assert to guarantee it
    assert(alignment > 0 && ((alignment & (alignment-1))) == 0);

    m_szBlockSize = ALIGN(minimal_size, alignment);
	
	// Storing m_szDataSize as blockSize_t right before each block
	assert(m_szBlockSize == (size_t)blockSize_t(m_szBlockSize));

    m_szAlignmentSize = m_szBlockSize - minimal_size;
	m_nBlocksPerPage = (m_szPageSize - sizeof(PageHeader)) / (m_szBlockSize + sizeof(BlockHeader));

	m_flag.clear(std::memory_order_release);
}

void* gswy::Allocator::Allocate() noexcept
{
	while (m_flag.test_and_set(std::memory_order_acquire)) { }
	if (!m_pFreeList)
	{
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
		auto size = static_cast<blockSize_t>(m_szBlockSize);
		// link each block in the page
		for (uint32_t i = 1; i < m_nBlocksPerPage; i++) {
			// Storing m_szBlockSize as blockSize_t right before each block
			pBlock->size = size;
			pBlock->free = true;
			pBlock->pNext = NextBlock(pBlock);
			pBlock = pBlock->pNext;
		}
		//link the last block
		pBlock->size = size;
		pBlock->free = true;
		pBlock->pNext = nullptr;

		m_pFreeList = pNewPage->Blocks();
	}
	BlockHeader* freeBlock = m_pFreeList;
	freeBlock->free = false;
	m_pFreeList = freeBlock->pNext;
	--m_nFreeBlocks;
#if defined(_DEBUG)
	FillAllocatedBlock(freeBlock);
#endif
	m_flag.clear(std::memory_order_release);

	/*
		return blockheader + 1 as the diagram below:
			 BlockHeader*  void*
			[Header block][User content]
	*/
    return Allocator::GetContent(freeBlock);
}

void gswy::Allocator::Free(void* p) noexcept
{
	while (m_flag.test_and_set(std::memory_order_acquire)) {}
    BlockHeader* block = reinterpret_cast<BlockHeader*>(p)-1;
	
	if (block->free)
	{
		throw std::runtime_error(std::string("Double free!"));
	}
	if (block->size != static_cast<blockSize_t>(m_szBlockSize))
	{
		throw std::runtime_error(std::string("Segementation fault!"));
	}
	block->free = true;
	block->pNext = m_pFreeList;
	m_pFreeList = block;
	++m_nFreeBlocks;
#if defined(_DEBUG)
	FillFreeBlock(block);
#endif
	m_flag.clear(std::memory_order_release);
}

void gswy::Allocator::FreeAll() noexcept
{
	while (m_flag.test_and_set(std::memory_order_acquire)) {}
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
	m_flag.clear(std::memory_order_release);
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
    return reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t*>(pBlock+1) + m_szBlockSize);
}