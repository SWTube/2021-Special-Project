/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Device/GenericWindow.h"

namespace cave
{
#ifdef __WIN32__
	GenericWindow::GenericWindow(uint32_t width, uint32_t height, const wchar_t* title)
#else
	GenericWindow::GenericWindow(uint32_t width, uint32_t height, const char* title)
#endif
		: mWidth(width)
		, mHeight(height)
		, mTitle(title)
	{
	}

	uint32_t GenericWindow::GetWidth() const
	{
		return mWidth;
	}

	uint32_t GenericWindow::GetHeight() const
	{
		return mHeight;
	}
}