/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"

#include "Memory/MemoryPool.h"

namespace cave
{
	constexpr size_t CORE_MEMORY_POOL_SIZE = 1048576ul;

	extern MemoryPool gCoreMemoryPool;
} // namespace cave
