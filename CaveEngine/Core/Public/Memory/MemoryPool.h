/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <vector>

#include "CoreTypes.h"
#include "Memory/DataBlock.h"

namespace cave
{
	class MemoryPool final
	{
	public:
		MemoryPool() = delete;
		MemoryPool(size_t maxPoolSize);
		MemoryPool(const MemoryPool&) = delete;
		virtual ~MemoryPool();

		MemoryPool& operator=(const MemoryPool& other) = delete;

		void* Allocate(size_t size);
		void Deallocate(void* item, size_t size);
		constexpr size_t GetFreeMemorySize() const;
		size_t GetCurrentStorage() const;
		size_t GetMaxNumDataBlocks() const;
		size_t GetPoolSize() const;
		void PrintPoolStatus() const;
		void PrintDataBlockByByte(size_t byte) const;
	private:
		size_t mPoolSize;
		size_t mFreeSize;
		size_t mMinBlockSize;
		size_t mMaxBlockSize;
		size_t mMaxNumDataBlocks;
		std::vector<DataBlock*> mDataBlocks;
	};

	constexpr size_t MemoryPool::GetFreeMemorySize() const
	{
		return mFreeSize;
	}

	FORCEINLINE constexpr size_t GetUpperPowerOfTwo(size_t number)
	{
		if (number && !(number & (number - 1ul)))
		{
			return number;
		}

		size_t count = 0ul;
		for (; number != 0ul; number >>= 1, ++count)
		{
		}

		return 1ul << count;
	}

	FORCEINLINE constexpr size_t GetExponent(size_t number)
	{
		if (!number || (number & (number - 1)))
		{
			return 0ul;
		}

		size_t exponent = 0ul;
		for (size_t i = 1ul; !(i & number); i <<= 1, ++exponent)
		{
		}

		return exponent;
	}

	FORCEINLINE constexpr size_t GetPowerOfTwo(size_t exponent)
	{
		size_t result = 1ul;

		for (size_t i = 0; i < exponent; ++i)
		{
			result <<= 1;
		}

		return result;
	}
}