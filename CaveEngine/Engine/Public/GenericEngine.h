/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"
#include "Device/DeviceResources.h"
#include "Renderer/Renderer.h"

namespace cave
{
	class GenericEngine
	{
	public:
		GenericEngine();
		virtual ~GenericEngine();

		virtual eResult Init(uint32_t screenWidth, uint32_t screenHeight) = 0;
		virtual void Destroy() = 0;

		virtual Window* GetWindowHandle();

		//virtual eResult Run(DeviceResources* deviceResources, Renderer* renderer);
		virtual eResult Run() = 0;

		virtual Renderer* GetRenderer();
	protected:
		MemoryPool* mPool = nullptr;
		Renderer* mRenderer = nullptr;
		Window* mWindow = nullptr;

		static const wchar_t* msWindowClassName;
	};
}