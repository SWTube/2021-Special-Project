/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __UNIX__
#include "GraphicsApiPch.h"

#include "Device/GenericWindow.h"

namespace cave
{
	class UnixWindow final : public GenericWindow
	{
	public:
		UnixWindow() = delete;
		UnixWindow(uint32_t width, uint32_t height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
		UnixWindow(const UnixWindow&) = delete;
		UnixWindow(const UnixWindow&&) = delete;
		UnixWindow& operator=(const UnixWindow&) = delete;
		virtual ~UnixWindow();

		void Resize(uint32_t width, uint32_t height) override;
		void OnChar(uint32_t codepoint);
		void OnKey(int32_t key, int32_t scancode, int32_t action, int32_t mods);

		GLFWwindow* GetWindow() override;
	private:
		static void windowSizeCallback(GLFWwindow* window, int32_t width, int32_t height);
		static void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void charCallback(GLFWwindow* window, uint32_t codepoint);

		GLFWmonitor* mMonitor = nullptr;
		GLFWwindow* mShare = nullptr;
		GLFWwindow* mWindow = nullptr;
	};

	typedef UnixWindow Window;
} // namespace cave
#endif