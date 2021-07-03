/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"
#include "Assertion/Assert.h"
#include "Containers/Node.h"

namespace cave
{
	class DataBlock final
	{
	public:
		DataBlock() = delete;
		DataBlock(size_t dataSize, size_t size);
		DataBlock(const DataBlock&) = delete;
		DataBlock(const DataBlock&&) = delete;
		DataBlock& operator=(const DataBlock&) = delete;
		DataBlock& operator=(const DataBlock&&) = delete;
		~DataBlock();

		bool IsEmpty() const;
		bool HasItem(void* item) const;
		FORCEINLINE void* Get();
		FORCEINLINE void Return(void* item);
		size_t GetSize() const;
		size_t GetFreeSize() const;
		size_t GetAllocatedSize() const;
		size_t GetPoolSize() const;
		void PrintFreedNodes() const;
		void PrintAllocatedNodes() const;
	private:
		FORCEINLINE void addItem(void* item);

		size_t mSize = 0u;
		size_t mFreeSize = 0u;
		size_t mAllocatedSize = 0u;
		MemoryNode* mFree = nullptr;
		MemoryNode* mAllocated = nullptr;
	};

	FORCEINLINE void* DataBlock::Get()
	{
		MemoryNode* allocatedNode = mFree;
		void* newPointer = allocatedNode->Data;
		mFree = mFree->Next;
		allocatedNode->Next = mAllocated;
		mAllocated = allocatedNode;
		++mAllocatedSize;
		--mFreeSize;
		
		return newPointer;
	}

	FORCEINLINE void DataBlock::Return(void* item)
	{
		if (mAllocated != nullptr)
		{
			MemoryNode* indirectIterator = mAllocated;

			if (indirectIterator->Data != item)
			{
				while (indirectIterator->Next != nullptr && indirectIterator->Next->Data != item)
				{
					indirectIterator = indirectIterator->Next;
				}

				if (indirectIterator->Next != nullptr)
				{
					MemoryNode* freedNode = indirectIterator->Next;
					indirectIterator->Next = indirectIterator->Next->Next;
					freedNode->Next = mFree;
					mFree = freedNode;
					--mAllocatedSize;
					++mFreeSize;
				}
				else
				{
					addItem(item);
				}
			}
			else
			{
				mAllocated = mAllocated->Next;
				MemoryNode* freedNode = indirectIterator;
				freedNode->Next = mFree;
				mFree = freedNode;
				--mAllocatedSize;
				++mFreeSize;
			}
		}
		else
		{
			addItem(item);
		}
	}

	FORCEINLINE void DataBlock::addItem(void* item)
	{
		assert(item != nullptr);

		MemoryNode* newNode = new MemoryNode(item, nullptr);

		if (mFree != nullptr)
		{
			newNode->Next = mFree;
			mFree = newNode;
		}
		else
		{
			mFree = newNode;
		}

		++mFreeSize;
	}
} // namespace neople