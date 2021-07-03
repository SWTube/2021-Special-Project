/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"
#include "GenericEngine.h"

#ifdef __WIN32__
namespace cave
{
	class WindowsEngine final : public GenericEngine
	{
	public:
		WindowsEngine() = default;
		virtual ~WindowsEngine() = default;

		eResult Init(uint32_t screenWidth, uint32_t screenHeight) override;
		void Destroy() override;
		eResult Run() override;

		static LRESULT CALLBACK StaticWindowProc(HWND hWindow, uint32_t message, WPARAM wParam, LPARAM lParam);
		
	private:
		static HINSTANCE	msInstance;
	};

	typedef WindowsEngine Engine;
}
#endif