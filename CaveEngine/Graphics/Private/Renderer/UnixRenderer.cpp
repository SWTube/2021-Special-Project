/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#ifdef __UNIX__
#include "lodepng.h"

#include "CoreMinimal.h"
#include "Renderer/UnixRenderer.h"

namespace cave
{
	//--------------------------------------------------------------------------------------
	// Destroy Renderer
	//--------------------------------------------------------------------------------------
	UnixRenderer::~UnixRenderer()
	{
		Destroy();
	}

	eResult UnixRenderer::Init(Window* window)
	{
		eResult result = CreateDeviceDependentResources();
		if (result != eResult::CAVE_OK)
		{
			return result;
		}
		result = CreateWindowSizeDependentResources(window);
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		return result;
	}

	//-----------------------------------------------------------------------------
	// Create device-dependent resources for rendering.
	//---------------------------------------------------------------------------
	eResult UnixRenderer::CreateDeviceDependentResources()
	{
		// Compile shaders using the Effects library.

		// Load the geometry for the spinning cube.

		// Instantiate the device manager class.
		mDeviceResources = reinterpret_cast<DeviceResources*>(mPool->Allocate(sizeof(DeviceResources)));
		new(mDeviceResources) DeviceResources(*mPool);
		// Create device resources.
		eResult result = mDeviceResources->CreateDeviceResources();
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		return result;
	}

	eResult UnixRenderer::CreateWindowSizeDependentResources(Window* window)
	{
		// We have a window, so initialize window size-dependent resources.
		eResult result = mDeviceResources->CreateWindowResources(window);
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		// set camera
		mCamera = reinterpret_cast<Camera*>(mPool->Allocate(sizeof(Camera)));
		new(mCamera) Camera();
		if (mCamera == nullptr)
		{
			return eResult::CAVE_OUT_OF_MEMORY;
		}
		mCamera->SetPosition(0.0f, 0.0f, -1.0f);

		// set color shader
		

		// set texture shader
		mShader = reinterpret_cast<Shader*>(mPool->Allocate(sizeof(Shader)));
		new(mShader) Shader("sprite.vert", "sprite.frag", *mPool);

		mShader->Compile();
		mDeviceResources->SetProgram(mShader->GetProgram());

		return result;
	}

	void UnixRenderer::Update()
	{
		for (Sprite* const object : mSprites)
		{
			object->Update();
		}

		++mFrameCount;
		if (mFrameCount == UINT32_MAX)
		{
			mFrameCount = 0u;
		}
	}

	//--------------------------------------------------------------------------------------
	// Clean up the objects we've created
	//--------------------------------------------------------------------------------------
	void UnixRenderer::cleanupDevice()
	{
	}

	bool UnixRenderer::WindowShouldClose()
	{
		return static_cast<bool>(glfwWindowShouldClose(mDeviceResources->GetWindow()->GetWindow()));
	}

	//--------------------------------------------------------------------------------------
	// Initialize Renderer
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	// Create OpenGL device and swap chain
	//--------------------------------------------------------------------------------------
	// Used to set up data for use later in the program
		// vertext information
		// image data
	// Specify shaders
	// Shader Plumbing

	//--------------------------------------------------------------------------------------
	// Render a frame
	//--------------------------------------------------------------------------------------
	void UnixRenderer::Render()
	{
		mDeviceResources->RenderStart();

		mCamera->Render();

		// glm::mat4& worldMatrix = mDeviceResources->GetWorldMatrix();
		const glm::mat4& viewMatrix = mCamera->GetViewMatrix();
		// glm::mat4& projection = mDeviceResources->GetProjectionMatrix();
		glm::mat4& ortho = mDeviceResources->GetOrthoMatrix();

		// 3. Set Render Data ---------------------------------------------------------------------------------------------
		for (Sprite* const object : mSprites)
		{
			object->Render();

			mShader->Render(object->GetIndicesCount(), viewMatrix, ortho, object->GetTextureIndex());
		}

		mDeviceResources->TurnZBufferOn();

		// Present the frame to the screen.
		mDeviceResources->RenderEnd();
	}

	void UnixRenderer::Destroy()
	{
		cleanupDevice();
		GenericRenderer::Destroy();
		mDeviceResources = nullptr;
	}
}
#endif