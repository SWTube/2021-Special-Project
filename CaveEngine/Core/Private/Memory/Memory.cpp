/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Memory/Memory.h"

namespace cave
{
	void* Memory::Malloc(size_t size)
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

#if !defined(__WIN32__)
	void* Memory::AlignedAlloc(size_t alignment, size_t size)
	{
		return aligned_alloc(alignment, size);
	}
#endif

	void* Memory::Calloc(size_t num, size_t size)
	{
		return calloc(num, size);
	}

	void* Memory::Realloc(void* ptr, size_t newSize)
	{
		return realloc(ptr, newSize);
	}

	void Memory::Free(void* ptr)
	{
		free(ptr);
	}

	int32_t Memory::Memcmp(const void* lhs, const void* rhs, size_t count)
	{
		return memcmp(lhs, rhs, count);
	}

	void* Memory::Memcpy(void* dest, const void* src, size_t count)
	{
		return memcpy(dest, src, count);
	}

	void* Memory::Memmove(void* dest, const void* src, size_t count)
	{
		return memmove(dest, src, count);
	}
} // namespace cave