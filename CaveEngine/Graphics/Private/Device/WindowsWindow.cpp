/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "CoreTypes.h"
#include "Debug/Log.h"

#include "Device/WindowsWindow.h"

#ifdef __WIN32__
namespace cave
{
	WindowsWindow::WindowsWindow(uint32_t width, uint32_t height, const wchar_t* title, HINSTANCE hInstance, LRESULT(CALLBACK* windowProc)(HWND, uint32_t, WPARAM, LPARAM))
		: GenericWindow(width, height, title)
	{
		// Window resources are dealt with here.
    
		if (hInstance == nullptr)
		{
			hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
		}

		// HICON hIcon = nullptr;
		// wchar_t szExePath[MAX_PATH];
		// GetModuleFileName(nullptr, szExePath, MAX_PATH);
		
		// // If the icon is NULL, then use the first one found in the exe
		// if(hIcon == NULL)
		// {
		// 	hIcon = ExtractIcon(hInstance, szExePath, 0);
		// }

		// Register the windows class
		WNDCLASSEX wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = windowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInstance;
		// wndClass.hIcon = hIcon;
		wndClass.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(107));
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = mTitle;
		wndClass.hIconSm = LoadIcon(wndClass.hInstance, reinterpret_cast<LPCTSTR>(107));

		if(!RegisterClassEx(&wndClass))
		{
			DWORD dwError = GetLastError();
			if(dwError != ERROR_CLASS_ALREADY_EXISTS)
			{
				assert(HRESULT_FROM_WIN32(dwError) == S_OK);
			}
		}

		// Create window
		mRect;
		int32_t x = CW_USEDEFAULT;
		int32_t y = CW_USEDEFAULT;

		// No menu in this example.
		mMenu = nullptr;

		// This example uses a non-resizable 640 by 480 viewport for simplicity.
		int32_t defaultWidth = 640;
		int32_t defaultHeight = 480;
		SetRect(&mRect, 0, 0, defaultWidth, defaultHeight);        
		AdjustWindowRect(
			&mRect,
			WS_OVERLAPPEDWINDOW,
			(mMenu != nullptr) ? true : false
		);

		// Create the window for our viewport.
		mWindow = CreateWindow(
			mTitle,
			L"CaveEngine",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			x, y,
			(mRect.right - mRect.left), (mRect.bottom - mRect.top),
			nullptr,
			mMenu,
			hInstance,
			nullptr
		);

		if (mWindow == nullptr)
		{
			DWORD dwError = GetLastError();
			assert(HRESULT_FROM_WIN32(dwError) == S_OK);
		}
	}

	HWND WindowsWindow::GetWindow()
	{
		return mWindow;
	}

	void WindowsWindow::Resize(uint32_t width, uint32_t height)
	{
	}
} // namespace cave
#endif