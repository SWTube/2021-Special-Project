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

#ifdef __WIN32__
	#include <d3d11_1.h>
#else
	#define GLFW_INCLUDE_NONE
	#include "GL/gl3w.h"
	#include "Glfw.h"
#endif