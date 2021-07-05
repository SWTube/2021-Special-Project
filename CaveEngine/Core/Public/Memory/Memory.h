/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <cstdlib>
#include <cstring>

#include "CoreTypes.h"

#include "Assertion/Assert.h"

namespace cave
{

	template <size_t N>
	FORCEINLINE constexpr size_t GetSufficientCapacity(size_t length)
	{
		assert(length <= SIZE_MAX - 1ul - N);

		return (length + 1ul + N - 1ul) - ((length + 1ul + N - 1ul) % N);
	}
	
	class Memory final
	{
	public:
		static void* Malloc(size_t size);
#if !defined(__WIN32__)
		static void* AlignedAlloc(size_t alignment, size_t size);
#endif
		static void* Calloc(size_t num, size_t size);
		static void* Realloc(void* ptr, size_t newSize);
		static void Free(void* ptr);
		static int32_t Memcmp(const void* lhs, const void* rhs, size_t count);
		constexpr static void* Memset(void* dest, int32_t fill, size_t count);
		static void* Memcpy(void* dest, const void* src, size_t count);
		static void* Memmove(void* dest, const void* src, size_t count);
	};

	inline uintptr_t AlignAddress(uintptr_t address, size_t align)
	{
		const size_t mask = align - 1;
		assert((align & mask) == 0);	// pwr of 2
		return (address + mask) & ~mask;
	}

	template<typename T>
	inline T* AlignPointer(T* ptr, size_t align)
	{
		const uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
		const uintptr_t alignedAddress = AlignAddress(address, align);
		return reinterpret_cast<T*>(alignedAddress);
	}

	constexpr void* Memory::Memset(void* dest, int32_t fill, size_t count)
	{
		return memset(dest, fill, count);
	}
} // namespace cave