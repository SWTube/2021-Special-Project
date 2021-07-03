/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"
#include "Device/GenericDeviceResources.h"

#ifdef __WIN32__
namespace cave
{
	class WindowsDeviceResources final : public GenericDeviceResources
	{
	public:
		WindowsDeviceResources(MemoryPool& pool);
		constexpr WindowsDeviceResources(const WindowsDeviceResources&) = delete;
		WindowsDeviceResources& operator=(const WindowsDeviceResources&) = delete;
		virtual ~WindowsDeviceResources();

		eResult Init(Window* window) override;
		eResult CreateWindowResources(Window* window) override;
		eResult CreateWindowResources(HWND hWindow);
		eResult CreateDeviceResources() override;
		void Destroy() override;

		int32_t ConfigureBackBuffer() override;
		int32_t ReleaseBackBuffer() override;
		int32_t GoFullScreen() override;
		int32_t GoWindowed() override;

		float GetAspectRatio() override;

		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RenderTargetView* GetRenderTarget();
		ID3D11DepthStencilView* GetDepthStencil();
		D3D_DRIVER_TYPE GetDriverType() const;

		virtual void GetVideoCardInfo(char* cardName, int& memory) override;

		virtual void TurnZBufferOn() override;
		virtual void TurnZBufferOff() override;

		virtual void RenderStart() override;
		virtual void RenderEnd() override;
	private:
		HINSTANCE	mInstance = nullptr;

		//-----------------------------------------------------------------------------
		// Direct3D device
		//-----------------------------------------------------------------------------
		ID3D11Device*        	mD3dDevice = nullptr;
		ID3D11Device1*       	mD3dDevice1 = nullptr;
		ID3D11DeviceContext*	mImmediateContext = nullptr; // immediate context
		ID3D11DeviceContext1*	mImmediateContext1 = nullptr;
		IDXGISwapChain*      	mSwapChain = nullptr;
		IDXGISwapChain1*      	mSwapChain1 = nullptr;


		//-----------------------------------------------------------------------------
		// DXGI swap chain device resources
		//-----------------------------------------------------------------------------
		ID3D11Texture2D*        mBackBuffer = nullptr;
		ID3D11RenderTargetView* mRenderTargetView = nullptr;


		//-----------------------------------------------------------------------------
		// Direct3D device resources for the depth stencil
		//-----------------------------------------------------------------------------
		ID3D11Texture2D*        	mDepthStencil = nullptr;
		ID3D11DepthStencilState*	mDepthStencilState = nullptr;
		ID3D11DepthStencilState*	mDepthDisabledStencilState = nullptr;
		ID3D11DepthStencilView*  	mDepthStencilView = nullptr;
		ID3D11RasterizerState*		mRasterizerState = nullptr;


		//-----------------------------------------------------------------------------
		// Direct3D device metadata and device resource metadata
		//-----------------------------------------------------------------------------
		D3D_FEATURE_LEVEL       mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		D3D11_TEXTURE2D_DESC    mBackBufferDesc;
		D3D11_VIEWPORT          mViewport;
		D3D_DRIVER_TYPE			mDriverType = D3D_DRIVER_TYPE_NULL;
	};

	typedef WindowsDeviceResources DeviceResources;
}
#endif