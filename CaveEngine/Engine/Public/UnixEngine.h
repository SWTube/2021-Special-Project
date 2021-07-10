/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __UNIX__
#include "CoreTypes.h"
#include "GenericEngine.h"

namespace cave
{
	class UnixEngine final : public GenericEngine
	{
	public:
		UnixEngine() = default;
		virtual ~UnixEngine() = default;

		eResult Init(uint32_t screenWidth, uint32_t screenHeight) override;
		void Destroy() override;
		eResult Run() override;
	};

	typedef UnixEngine Engine;
}
#endif