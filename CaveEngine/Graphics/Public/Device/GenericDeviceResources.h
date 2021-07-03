/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"
#include "Device/Window.h"
#include "Memory/MemoryPool.h"

namespace cave
{
	class GenericDeviceResources
	{
	public:
		GenericDeviceResources(MemoryPool& pool);
		constexpr GenericDeviceResources(const GenericDeviceResources&) = default;
		GenericDeviceResources& operator=(const GenericDeviceResources&) = default;
		virtual ~GenericDeviceResources() = default;

		virtual eResult Init(Window* window) = 0;
		virtual eResult CreateWindowResources(Window* window) = 0;
		virtual eResult CreateDeviceResources() = 0;
		virtual void Destroy() = 0;

		virtual int32_t ConfigureBackBuffer() = 0;
		virtual int32_t ReleaseBackBuffer() = 0;
		virtual int32_t GoFullScreen() = 0;
		virtual int32_t GoWindowed() = 0;

		virtual float GetAspectRatio() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		constexpr MemoryPool* GetMemoryPool();
		constexpr Window* GetWindow();

#ifdef __WIN32__
		constexpr virtual DirectX::XMMATRIX& GetProjectionMatrix();
		constexpr virtual DirectX::XMMATRIX& GetWorldMatrix();
		constexpr virtual DirectX::XMMATRIX& GetOrthoMatrix();
#else
		constexpr virtual glm::mat4& GetProjectionMatrix();
		constexpr virtual glm::mat4& GetWorldMatrix();
		constexpr virtual glm::mat4& GetOrthoMatrix();
#endif

		virtual void GetVideoCardInfo(char* cardName, int& memory) = 0;

		virtual void TurnZBufferOn() = 0;
		virtual void TurnZBufferOff() = 0;

		virtual void RenderStart() = 0;
		virtual void RenderEnd() = 0;
	protected:
		MemoryPool* mPool = nullptr;
		Window* mWindow = nullptr;
		bool mbVsyncEnabled = false;
		int32_t mVideoCardMemory = 0;
		char mVideoCardDescription[128] = {'\0', };
#ifdef __WIN32__
		DirectX::XMMATRIX mProjection = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX mWorld = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX mOrtho = DirectX::XMMatrixIdentity();
#else
		glm::mat4 mProjection = glm::mat4(1.0f);
		glm::mat4 mWorld = glm::mat4(1.0f);
		glm::mat4 mOrtho= glm::mat4(1.0f);
#endif
	};

#ifdef __WIN32__
	constexpr DirectX::XMMATRIX& GenericDeviceResources::GetProjectionMatrix()
#else
	constexpr glm::mat4& GenericDeviceResources::GetProjectionMatrix()
#endif
	{
		return mProjection;	
	}

#ifdef __WIN32__
	constexpr DirectX::XMMATRIX& GenericDeviceResources::GetWorldMatrix()
#else
	constexpr glm::mat4& GenericDeviceResources::GetWorldMatrix()
#endif
	{
		return mWorld;
	}

#ifdef __WIN32__
	constexpr DirectX::XMMATRIX& GenericDeviceResources::GetOrthoMatrix()
#else
	constexpr glm::mat4& GenericDeviceResources::GetOrthoMatrix()
#endif
	{
		return mOrtho;
	}

	constexpr MemoryPool* GenericDeviceResources::GetMemoryPool()
	{
		return mPool;
	}

	constexpr Window* GenericDeviceResources::GetWindow()
	{
		return mWindow;
	}
}