/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "Device/GenericWindow.h"

#ifdef __WIN32__
namespace cave
{
	class WindowsWindow final : public GenericWindow
	{
	public:
		WindowsWindow() = delete;
		WindowsWindow(uint32_t width, uint32_t height, const wchar_t* title, HINSTANCE hInstance, LRESULT (CALLBACK* windowProc)(HWND, uint32_t, WPARAM, LPARAM));
		WindowsWindow(const WindowsWindow&) = delete;
		WindowsWindow(const WindowsWindow&&) = delete;
		WindowsWindow& operator=(const WindowsWindow&) = delete;
		virtual ~WindowsWindow() = default;

		void Resize(uint32_t width, uint32_t height) override;
		HWND GetWindow() override;
	private:
		HWND mWindow = nullptr;
		HMENU mMenu = nullptr;
		RECT mRect;
	};

	typedef WindowsWindow Window;
} // namespace cave
#endif