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
	class Memory final
	{
	public:
		constexpr static void* Malloc(size_t size);
		constexpr static void* AlignedAlloc(size_t alignment, size_t size);
		constexpr static void* Calloc(size_t num, size_t size);
		constexpr static void* Realloc(void* ptr, size_t newSize);
		constexpr static void Free(void* ptr);
		constexpr static int32_t Memcmp(const void* lhs, const void* rhs, size_t count);
		constexpr static void* Memset(void* dest, int32_t fill, size_t count);
		constexpr static void* Memcpy(void* dest, const void* src, size_t count);
		constexpr static void* Memmove(void* dest, const void* src, size_t count);
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

	constexpr void* Memory::Malloc(size_t size)
	{
		// size:	number of bytes to allocate 

		//Allocates size bytes of uninitialized storage.
		//
		//If allocation succeeds, returns a pointer to the lowest(first) byte in the allocated memory block that is suitably aligned for any scalar type.
		//
		//If size is zero, the behavior is implementation defined(null pointer may be returned, or some non - null pointer may be returned that may not be used to access storage, but has to be passed to std::free)
		//The following functions are required to be thread - safe:

		//The library versions of operator new and operator delete
		//	User replacement versions of global operator new and operator delete
		//	std::calloc, std::malloc, std::realloc, std::aligned_alloc(since C++17), std::free

		//	Calls to these functions that allocate or deallocate a particular unit of storage occur in a single total order, and each such deallocation call happens - before the next allocation(if any) in this order.

		// return:	On success, returns the pointer to the beginning of newly allocated memory. To avoid a memory leak, the returned pointer must be deallocated with std::free() or std::realloc().
					//On failure, returns a null pointer.
		return malloc(size);
	}

	constexpr void* Memory::AlignedAlloc(size_t alignment, size_t size)
	{
		return aligned_alloc(alignment, size);
	}

	constexpr void* Memory::Calloc(size_t num, size_t size)
	{
		return calloc(num, size);
	}

	constexpr void* Memory::Realloc(void* ptr, size_t newSize)
	{
		return realloc(ptr, newSize);
	}

	constexpr void Memory::Free(void* ptr)
	{
		free(ptr);
	}

	constexpr int32_t Memory::Memcmp(const void* lhs, const void* rhs, size_t count)
	{
		return memcmp(lhs, rhs, count);
	}

	constexpr void* Memory::Memset(void* dest, int32_t fill, size_t count)
	{
		return memset(dest, fill, count);
	}

	constexpr void* Memory::Memcpy(void* dest, const void* src, size_t count)
	{
		return memcpy(dest, src, count);
	}

	constexpr void* Memory::Memmove(void* dest, const void* src, size_t count)
	{
		return memmove(dest, src, count);
	}
} // namespace cave