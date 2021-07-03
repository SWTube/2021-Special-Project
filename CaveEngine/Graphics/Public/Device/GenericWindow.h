/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"

namespace cave
{
	class GenericWindow
	{
	public:
		GenericWindow() = delete;
#ifdef __WIN32__
		GenericWindow(uint32_t width, uint32_t height, const wchar_t* title);
#else
		GenericWindow(uint32_t width, uint32_t height, const char* title);
#endif
		GenericWindow(const GenericWindow&) = delete;
		GenericWindow(const GenericWindow&&) = delete;
		GenericWindow& operator=(const GenericWindow&) = delete;
		virtual ~GenericWindow() = default;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

#ifdef __WIN32__
		virtual HWND GetWindow() = 0;
#else
		virtual GLFWwindow* GetWindow() = 0;
#endif
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		constexpr float GetNear() const;
		constexpr float GetFar() const;

		constexpr bool IsFullScreen() const;
		constexpr void SetFullScreen();
		constexpr void SetWindowed();
	protected:
		bool mbIsFullScreen = false;
		uint32_t mWidth = 0u;
		uint32_t mHeight = 0u;
#ifdef __WIN32__
		float mNear = 0.1f;
		float mFar = 1000.0f;
#else
		float mNear = -1.0f;
		float mFar = 1.0f;
#endif
		
#ifdef __WIN32__
		const wchar_t* mTitle = nullptr;
#else
		const char* mTitle = nullptr;
#endif
	};

	constexpr bool GenericWindow::IsFullScreen() const
	{
		return mbIsFullScreen;
	}

	constexpr void GenericWindow::SetFullScreen()
	{
		mbIsFullScreen = true;
	}

	constexpr void GenericWindow::SetWindowed()
	{
		mbIsFullScreen = false;
	}

	constexpr float GenericWindow::GetNear() const
	{
		return mNear;
	}

	constexpr float GenericWindow::GetFar() const
	{
		return mFar;
	}
}
