/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Renderer/WindowsRenderer.h"

#ifdef __WIN32__
#include <ppltasks.h>

#include "Debug/Log.h"
import DdsTextureLoader;

namespace cave
{
	//--------------------------------------------------------------------------------------
	// Destroy Renderer
	//--------------------------------------------------------------------------------------
	WindowsRenderer::~WindowsRenderer()
	{
		Destroy();
	}

	eResult WindowsRenderer::Init(Window* window)
	{
		CreateDeviceDependentResources();
		CreateWindowSizeDependentResources(window);
		// set camera
		mCamera = reinterpret_cast<Camera*>(mPool->Allocate(sizeof(Camera)));
		new(mCamera) Camera();
		if (mCamera == nullptr)
		{
			return eResult::CAVE_OUT_OF_MEMORY;
		}
		mCamera->SetPosition(0.0f, 0.0f, -100.f);
		// set color shader
		// set texture shader
		mShader = reinterpret_cast<Shader*>(mPool->Allocate(sizeof(Shader)));
		new(mShader) cave::Shader(L"DirectXTest.fxh", *mPool);
		mShader->Compile(mDeviceResources->GetDevice());
	}

	//--------------------------------------------------------------------------------------
	// Destroy Renderer
	//--------------------------------------------------------------------------------------
	void WindowsRenderer::Destroy()
	{
		if (mConstantBufferNeverChanges)
		{
			mConstantBufferNeverChanges->Release();
		}
		if (mConstantBufferChangeOnResize)
		{
			mConstantBufferChangeOnResize->Release();
		}

		GenericRenderer::Destroy();
	}

	//--------------------------------------------------------------------------------------
	// Render a frame
	//--------------------------------------------------------------------------------------
	void WindowsRenderer::Render()
	{
		ID3D11DeviceContext* context = mDeviceResources->GetDeviceContext();

		ID3D11RenderTargetView* renderTarget = mDeviceResources->GetRenderTarget();
		ID3D11DepthStencilView* depthStencil = mDeviceResources->GetDepthStencil();

		mDeviceResources->RenderStart();

		mCamera->Render();

		DirectX::XMMATRIX& worldMatrix = mDeviceResources->GetWorldMatrix();
		const DirectX::XMMATRIX& viewMatrix = mCamera->GetViewMatrix();
		DirectX::XMMATRIX& projection = mDeviceResources->GetProjectionMatrix();
		DirectX::XMMATRIX& ortho = mDeviceResources->GetOrthoMatrix();

		// 3. Set Render Data ---------------------------------------------------------------------------------------------
		for (Sprite* const object : mSprites)
		{
			object->Render();

			mShader->Render(context, object->GetIndicesCount(), worldMatrix, viewMatrix, ortho, mTextures[object->GetTextureIndex()]->GetTexture());
		}

		// ���� flip�̳� swap ���ؼ� front buffer�� ����
		// front buffer�� ������
		// ���� �׷����� ������ GPU Driver���� �ٸ�. ������ ���������� �񵿱�� �Ͼ
		// Present the information rendered to the back buffer to the front buffer (the screen)
		// gSwapChain->Present(0, 0);

		mDeviceResources->TurnZBufferOn();
		// Present the frame to the screen.
		mDeviceResources->RenderEnd();
	}

	eResult WindowsRenderer::CreateDeviceDependentResources()
	{
		// Instantiate the device manager class.
		mDeviceResources = reinterpret_cast<DeviceResources*>(mPool->Allocate(sizeof(DeviceResources)));
		new(mDeviceResources) DeviceResources(*mPool);
		// Create device resources.
		eResult result = mDeviceResources->CreateDeviceResources();
		if (result != eResult::CAVE_OK)
		{
			return result;
		}
	}

	eResult WindowsRenderer::CreateWindowSizeDependentResources(Window* window)
	{
		// We have a window, so initialize window size-dependent resources.

		return 	mDeviceResources->CreateWindowResources(window);
	}

	void WindowsRenderer::Update()
	{
		for (Sprite* const object : mSprites)
		{
			object->Update();
		}

		++mFrameCount;
		if (mFrameCount == UINT32_MAX)
		{
			mFrameCount == 0u;
		}
	}

	// eResult WindowsRenderer::createShaders()
	// {
	// 	ID3D11Device* device = mDeviceResources->GetDevice();
	// 	for (Shader& shader : mShaders)
	// 	{
	// 		if (eResult result = shader.Compile(mDeviceResources->GetDevice()); result != eResult::CAVE_OK)
	// 		{
	// 			return result;
	// 		}
	// 	}

	// 	// Create the constant buffers
	// 	D3D11_BUFFER_DESC bd = {};
	// 	bd.Usage = D3D11_USAGE_DEFAULT;
	// 	bd.ByteWidth = sizeof(ConstantBufferNeverChanges);
	// 	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 	bd.CPUAccessFlags = 0;
	// 	int32_t result = device->CreateBuffer(&bd, nullptr, &mConstantBufferNeverChanges);
	// 	if(FAILED(result))
	// 	{
	// 		return eResult::CAVE_FAIL;
	// 	}
		
	// 	bd.ByteWidth = sizeof(ConstantBufferChangeOnResize);
	// 	result = device->CreateBuffer(&bd, nullptr, &mConstantBufferChangeOnResize);
	// 	if(FAILED(result))
	// 	{
	// 		return eResult::CAVE_FAIL;
	// 	}
		
	// 	device->Release();

	// 	return eResult::CAVE_OK;
	// }

	bool WindowsRenderer::WindowShouldClose()
	{
		return false;
	}
}
#endif