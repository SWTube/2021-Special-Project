/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Time/TimeManager.h"

namespace cave
{
	TimeManager::TimeManager()
	{
		mCurrentTimePoint = std::chrono::steady_clock::now();
	}

	float TimeManager::GetDeltaTime()
	{
		std::chrono::steady_clock::time_point newCurrentTimePoint = std::chrono::steady_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(newCurrentTimePoint - mCurrentTimePoint);
		mCurrentTimePoint = newCurrentTimePoint;

		return static_cast<float>(deltaTime.count() * 0.001);
	}

	std::chrono::steady_clock::time_point TimeManager::GetTimePoint() const
	{
		return mCurrentTimePoint;
	}
}