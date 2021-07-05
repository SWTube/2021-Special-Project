/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Memory/DataBlock.h"
#include "Memory/Memory.h"

namespace cave
{
	DataBlock::DataBlock(size_t dataSize, size_t size)
		: mSize(dataSize)
	{
		for (size_t i = 0ul; i < size; ++i)
		{
			// push back memory allocated according to the size of Data Block
			void* newPointer = Memory::Malloc(mSize);
			assert(newPointer != nullptr);
			addItem(newPointer);
		}
	}

	DataBlock::~DataBlock()
	{
		while (mFree != nullptr)
		{
			MemoryNode* tempNode = mFree;
			mFree = mFree->Next;
			delete tempNode;
		}

#ifdef CAVE_BUILD_DEBUG
		assert (mAllocated == nullptr);
#endif
		while (mAllocated != nullptr)
		{
			MemoryNode* tempNode = mAllocated;
			mAllocated = mAllocated->Next;
			delete tempNode;
		}
	}

	bool DataBlock::IsEmpty() const
	{
		return mFree == nullptr;
	}

	bool DataBlock::HasItem(void* item) const
	{
		MemoryNode* iterator = mFree;
		while (iterator != nullptr)
		{
			if (iterator->Data == item)
			{
				return true;
			}
			iterator = iterator->Next;
		}

		return false;
	}

	size_t DataBlock::GetSize() const
	{
		return mSize;
	}

	size_t DataBlock::GetFreeSize() const
	{
		return mFreeSize;
	}

	size_t DataBlock::GetAllocatedSize() const
	{
		return mAllocatedSize;
	}

	size_t DataBlock::GetPoolSize() const
	{
		return mSize * mFreeSize;
	}

	void DataBlock::PrintFreedNodes() const
	{
		MemoryNode* iterator = mFree;

		while (iterator != nullptr)
		{
			//LOGDF(eLogChannel::CORE_MEMORY, "Freed Node: %s", iterator->Data);
			iterator = iterator->Next;
		}
	}

	void DataBlock::PrintAllocatedNodes() const
	{
		MemoryNode* iterator = mAllocated;
		size_t i = 0ul;

		while (iterator != nullptr)
		{
			//LOGDF(eLogChannel::CORE_MEMORY, "%d: Allocated Node: %s", i, iterator->Data);
			iterator = iterator->Next;
			++i;
		}
	}
}