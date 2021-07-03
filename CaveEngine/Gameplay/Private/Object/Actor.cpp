/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Object/Actor.h"

namespace cave
{
	Actor::Actor()
	{
		initialize();
	}

	void Actor::initialize()
	{

		mbCanCollision = true;


		mPlayState = ActorStateType::Play::None;

		mbIsBeingDestroyed = false;

		mbHasFinishedSpawning = false;

		mbCameraViewTarget = false;

		mbVisible = true;

		mbInitailized = true;
	}
}