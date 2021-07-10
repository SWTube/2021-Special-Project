/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#ifdef __UNIX__
#include "CoreTypes.h"
#include "Debug/Log.h"

#include "Device/UnixWindow.h"

namespace cave
{
	UnixWindow::UnixWindow(uint32_t width, uint32_t height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
		: GenericWindow(width, height, title)
		, mMonitor(monitor)
		, mShare(share)
		, mWindow(glfwCreateWindow(static_cast<int32_t>(mWidth), static_cast<int32_t>(mHeight), mTitle, mMonitor, mShare))
	{
		glfwSetWindowUserPointer(mWindow, this);
		glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
		glfwSetKeyCallback(mWindow, keyCallback);
		glfwSetCharCallback(mWindow, charCallback);
	}

	UnixWindow::~UnixWindow()
	{
		glfwDestroyWindow(mWindow);
	}

	void UnixWindow::OnChar(uint32_t codepoint)
	{
		LOGIF(eLogChannel::GRAPHICS, "Codepoint: %u", codepoint);
	}

	void UnixWindow::OnKey(int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		LOGIF(eLogChannel::GRAPHICS, "Key: %d, Scancode: %d, Action: %d, Mods: %d", key, scancode, action, mods);
	}

	void UnixWindow::Resize(uint32_t width, uint32_t height)
	{
		mWidth = width;
		mHeight = height;
		glViewport(0, 0, static_cast<int32_t>(mWidth), static_cast<int32_t>(mHeight));
	}

	GLFWwindow* UnixWindow::GetWindow()
	{
		return mWindow;
	}

	void UnixWindow::windowSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
	{	
		reinterpret_cast<UnixWindow*>(glfwGetWindowUserPointer(window))->Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	}

	void UnixWindow::keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		reinterpret_cast<UnixWindow*>(glfwGetWindowUserPointer(window))->OnKey(key, scancode, action, mods);
	}

	void UnixWindow::charCallback(GLFWwindow* window, uint32_t codepoint)
	{
		reinterpret_cast<UnixWindow*>(glfwGetWindowUserPointer(window))->OnChar(codepoint);
	}
} // namespace cave
#endif