/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once	

#include "Core.h"

namespace cave
{
	namespace ActorStateType
	{
		enum class Life
		{
			Forever,
			Expired,
		};

		enum class Play
		{
			None,
			Start,
			End,
		};
	}
}