/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __UNIX__
#include "GraphicsApiPch.h"

#include "CoreTypes.h"
#include "Device/GenericDeviceResources.h"

namespace cave
{
	class UnixDeviceResources final : public GenericDeviceResources
	{
	public:
		UnixDeviceResources(MemoryPool& pool);
		constexpr UnixDeviceResources(const UnixDeviceResources&) = default;
		UnixDeviceResources& operator=(const UnixDeviceResources&) = default;
		virtual ~UnixDeviceResources() = default;

		eResult Init(Window* window) override;
		eResult CreateDeviceResources() override;
		eResult CreateWindowResources(Window* window) override;
		eResult CreateWindowResources(GLFWwindow* window);
		void Destroy() override;

		int32_t ConfigureBackBuffer() override;
		int32_t ReleaseBackBuffer() override;
		int32_t GoFullScreen() override;
		int32_t GoWindowed() override;

		float GetAspectRatio() override;

		uint32_t GetProgram() const;
		void SetProgram(uint32_t program);

		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;

		virtual void GetVideoCardInfo(char* cardName, int& memory) override;

		virtual void TurnZBufferOn() override;
		virtual void TurnZBufferOff() override;

		virtual void RenderStart() override;
		virtual void RenderEnd() override;
	private:
		static void errorCallback(int32_t errorCode, const char* description);

		//-----------------------------------------------------------------------------
		// OpenGL device
		//-----------------------------------------------------------------------------
		uint32_t	mProgram = 0u;
		uint32_t	mWidth = 0u;
		uint32_t	mHeight = 0u;
	};

	typedef UnixDeviceResources DeviceResources;
}
#endif