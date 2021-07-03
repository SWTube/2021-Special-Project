/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#if !defined(WIN32)
	#include "CaveEngineConfig.h"
#else
	 /*--------------------------------------------------------------------------------
		 Build configuration coming from CAVE, do not modify
	 --------------------------------------------------------------------------------*/

	 // Ensure that we have one, and only one build config coming from UBT
	#if CAVE_BUILD_DEBUG + CAVE_BUILD_DEVELOPMENT + CAVE_BUILD_TEST + CAVE_BUILD_RELEASE != 1
		#error Exactly one of [CAVE_BUILD_DEBUG CAVE_BUILD_DEVELOPMENT CAVE_BUILD_TEST CAVE_BUILD_RELEASE] should be defined to be 1
	#endif

	#ifndef __WIN32__
		#define __WIN32__
	#endif

	#define PLATFORM_NAME Windows

	#define __FORCEINLINE__ __forceinline
#endif

#define GET_PLATFORM_HEADER(x) STRINGIZE_TOKEN(CONCATENATE_TOKENS(PLATFORM_NAME, x))

// Turns an preprocessor token into a real string
#define STRINGIZE_TOKEN(x) STRINGIZE_TOKEN_IMPL(x)
#define STRINGIZE_TOKEN_IMPL(x) #x

// Concatenates two preprocessor tokens, performing macro expansion on them first
#define CONCATENATE_TOKENS(x, y) CONCATENATE_TOKENS_IMPL(x, y)
#define CONCATENATE_TOKENS_IMPL(x, y) x##y

// Expands to the second argument or the third argument if the first argument is 1 or 0 respectively
#define IF(cond, x, y) CONCATENATE_TOKENS(IF_IMPL_, cond)(x, y)
#define IF_IMPL_0(x, y) x
#define IF_IMPL_1(x, y) y

#ifdef CAVE_BUILD_DEBUG
	#ifdef __UNIX__
		#define FORCEINLINE __attribute__((always_inline)) inline
	#else
		#define FORCEINLINE __forceinline
	#endif
#else
#endif

#include <float.h>
#include <stdint.h>
#include <stddef.h>
#include <utility>

namespace cave
{
	enum class eResult : uint32_t
	{
		CAVE_OK				= 0x00000000,
		CAVE_ABORT			= 0x80004004,
		CAVE_ACCESS_DENIED	= 0x80070005,
		CAVE_FAIL			= 0x80004005,
		CAVE_HANDLE_ERROR	= 0x80070006,
		CAVE_INVALID_ARG	= 0x80070057,
		CAVE_NO_INTERFACE	= 0x80004002,
		CAVE_NOT_IMPL		= 0x80004001,
		CAVE_OUT_OF_MEMORY	= 0x8007000E,
		CAVE_POINTER_ERROR	= 0x80004003,
		CAVE_UNEXPECTED		= 0x8000FFFF,
	};
}