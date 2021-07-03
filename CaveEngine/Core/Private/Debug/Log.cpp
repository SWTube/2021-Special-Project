/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Debug/Log.h"
#include "String/String.h"

#ifdef __UNIX__
namespace cave
{
	eLogVerbosity LogManager::msCurrentVerbosity = eLogVerbosity::All;
	char LogManager::mBuffer[MAX_BUFFER] = {'\0', };

	void LogManager::SetVerbosity(eLogVerbosity verbosity)
	{
		msCurrentVerbosity = verbosity;
	}

	void LogManager::Verbose(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Verbose, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::VerboseF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Verbose, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::Debug(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Debug, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::DebugF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Debug, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::Info(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Info, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::InfoF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Info, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::Warn(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Warn, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::WarnF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Warn, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::Error(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Error, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::ErrorF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Error, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::Assert(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		log(channel, eLogVerbosity::Assert, fileName, functionName, lineNumber, os, message);
	}

	void LogManager::AssertF(eLogChannel channel, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message, ...)
	{
		va_list vl;
		va_start(vl, message);
		vsnprintf(mBuffer, MAX_BUFFER, message, vl);
		log(channel, eLogVerbosity::Assert, fileName, functionName, lineNumber, os, mBuffer);
		va_end(vl);
	}

	void LogManager::log(eLogChannel channel, eLogVerbosity verbosity, const char* fileName, const char* functionName, int32_t lineNumber, std::ostream& os, const char* message)
	{
		if (msCurrentVerbosity == eLogVerbosity::All || verbosity == msCurrentVerbosity)
		{
			String buffer;

			switch (channel)
			{
			case eLogChannel::GRAPHICS:
				buffer = "Graphics/";
				break;
			case eLogChannel::PHYSICS:
				buffer = "Physics/";
				break;
			case eLogChannel::AUDIO:
				buffer = "Audio/";
				break;
			case eLogChannel::AI:
				buffer = "AI/";
				break;
			case eLogChannel::GAMEPLAY:
				buffer = "Gameplay/";
				break;
			case eLogChannel::CORE:
				buffer = "Core/";
				break;
			case eLogChannel::CORE_MODULE:
				buffer = "Core/Module/";
				break;
			case eLogChannel::CORE_UNIT_TEST:
				buffer = "Core/UnitTest/";
				break;
			case eLogChannel::CORE_MEMORY:
				buffer = "Core/Memory/";
				break;
			case eLogChannel::CORE_MATH:
				buffer = "Core/Math/";
				break;
			case eLogChannel::CORE_STRING:
				buffer = "Core/String/";
				break;
			case eLogChannel::CORE_LOCALIZATION:
				buffer = "Core/Localization/";
				break;
			case eLogChannel::CORE_PARSER:
				buffer = "Core/Parser/";
				break;
			case eLogChannel::CORE_PROFILE:
				buffer = "Core/Profile/";
				break;
			case eLogChannel::CORE_ENGINE_CONFIG:
				buffer = "Core/EngineConfig/";
				break;
			case eLogChannel::CORE_RNG:
				buffer = "Core/RandomNumberGenerator/";
				break;
			case eLogChannel::CORE_OBJECT:
				buffer = "Core/Object/";
				break;
			case eLogChannel::CORE_THREAD:
				buffer = "Core/Thread/";
				break;
			case eLogChannel::CORE_CONTAINER:
				buffer = "Core/Container/";
				break;
			case eLogChannel::CORE_FILE_SYSTEM:
				buffer = "Core/FileSystem/";
				break;
			case eLogChannel::CORE_TIMER:
				buffer = "Core/Timer/";
				break;
			case eLogChannel::CORE_RESOURCE_MANAGER:
				buffer = "Core/ResourceManager/";
				break;
			default:
				assert(false);
				break;
			}

			char color = '7';
			switch (verbosity)
			{
			case eLogVerbosity::Verbose:
				buffer += "V/";
				break;
			case eLogVerbosity::Debug:
				buffer += "D/";
				color = '2';
				break;
			case eLogVerbosity::Info:
				buffer += "I/";
				color = '3';
				break;
			case eLogVerbosity::Warn:
				buffer += "W/";
				color = '5';
				break;
			case eLogVerbosity::Error:
				buffer += "E/";
				color = '1';
				break;
			case eLogVerbosity::Assert:
				buffer += "A/";
				color = '6';
				break;
			default:
				assert(false);
				break;
			}

			os << "\033[1;3" << color << 'm' << buffer << fileName << "/" << functionName << "/line:" << lineNumber << " :\t" << message << "\033[0m" << std::endl;
		}
	}
} // namespace cave
#endif