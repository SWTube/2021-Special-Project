/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __WIN32__
	import Log;
#else
	#include <cstdarg>
	#include <iostream>

	#include "CoreTypes.h"
	
	namespace cave
	{
		constexpr size_t MAX_BUFFER = 255ul;

		enum class eLogVerbosity
		{
			All,
			Verbose,
			Debug,
			Info,
			Warn,
			Error,
			Assert
		};

		enum class eLogChannel
		{
			GRAPHICS = 0x00,
			PHYSICS = 0x20,
			AUDIO = 0x40,
			AI = 0x60,
			GAMEPLAY = 0x80,
			CORE = 0xa0,
			CORE_MODULE = 0xa1,
			CORE_UNIT_TEST = 0xa2,
			CORE_MEMORY = 0xa3,
			CORE_MATH = 0xa4,
			CORE_STRING = 0xa5,
			CORE_LOCALIZATION = 0xa6,
			CORE_PARSER = 0xa7,
			CORE_PROFILE = 0xa8,
			CORE_ENGINE_CONFIG = 0xa9,
			CORE_RNG = 0xaa,
			CORE_OBJECT = 0xab,
			CORE_THREAD = 0xac,
			CORE_CONTAINER = 0xad,
			CORE_FILE_SYSTEM = 0xae,
			CORE_TIMER = 0xaf,
			CORE_RESOURCE_MANAGER = 0xb0
		};

		class Log final
		{
		public:
			Log() = delete;
			Log(const Log&) = delete;
			Log(const Log&&) = delete;
			Log& operator=(const Log&) = delete;
			Log& operator=(const Log&&) = delete;

			static void SetVerbosity(eLogVerbosity verbosity);
			static void Verbose(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void VerboseF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
			static void Debug(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void DebugF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
			static void Info(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void InfoF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
			static void Warn(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void WarnF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
			static void Error(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void ErrorF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
			static void Assert(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static void AssertF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, const char* message, ...);
		private:
			static void log(eLogChannel channel, eLogVerbosity verbosity, const char* fileName, const char* functionName, int32_t lineNumber, const char* message);
			static eLogVerbosity msCurrentVerbosity;
			static char mBuffer[MAX_BUFFER];
		};
	} // namespace cave
#endif

#define LOGVF(channel, message, ...) cave::Log::VerboseF(channel, __FILE__, __func__, __LINE__, message, __VA_ARGS__)
#define LOGV(channel, message) cave::Log::Verbose(channel, __FILE__, __func__, __LINE__, message)
#define LOGDF(channel, message, ...) cave::Log::DebugF(channel, __FILE__, __func__, __LINE__, message, __VA_ARGS__)
#define LOGD(channel, message) cave::Log::Debug(channel, __FILE__, __func__, __LINE__, message)
#define LOGIF(channel, message, ...) cave::Log::InfoF(channel, __FILE__, __func__, __LINE__, message, __VA_ARGS__)
#define LOGI(channel, message) cave::Log::Info(channel, __FILE__, __func__, __LINE__, message)
#define LOGWF(channel, message, ...) cave::Log::WarnF(channel, __FILE__,__func__,  __LINE__, message, __VA_ARGS__)
#define LOGW(channel, message) cave::Log::Warn(channel, __FILE__,__func__,  __LINE__, message)
#define LOGEF(channel, message, ...) cave::Log::ErrorF(channel, __FILE__, __func__, __LINE__, message, __VA_ARGS__)
#define LOGE(channel, message) cave::Log::Error(channel, __FILE__, __func__, __LINE__, message)
#define LOGAF(channel, message, ...) cave::Log::AssertF(channel, __FILE__, __func__, __LINE__, message, __VA_ARGS__)
#define LOGA(channel, message) cave::Log::Assert(channel, __FILE__, __func__, __LINE__, message)