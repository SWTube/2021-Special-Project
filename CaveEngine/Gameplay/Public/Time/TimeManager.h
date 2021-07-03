/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <chrono>
//#include <Windows.h>

#include "Core.h"

namespace cave
{
	class TimeManager
	{
	public:
		/** Default TickManager constructor. */
		TimeManager();

		float GetDeltaTime();

		std::chrono::steady_clock::time_point GetTimePoint() const;

	private:
		std::chrono::steady_clock::time_point mCurrentTimePoint;
	};
}