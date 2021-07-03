/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Device/WindowsDeviceResources.h"

#ifdef __WIN32__
namespace cave
{
	WindowsDeviceResources::WindowsDeviceResources(MemoryPool& pool)
		: GenericDeviceResources(pool)
	{
	}

	WindowsDeviceResources::~WindowsDeviceResources()
	{
		Destroy();
	}

	eResult WindowsDeviceResources::Init(Window* window)
	{
		eResult result = CreateDeviceResources();
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		result = CreateWindowResources(window);
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		return result;
	}

	eResult WindowsDeviceResources::CreateDeviceResources()
	{
		int32_t result = S_OK;

		uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(CAVE_BUILD_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		uint32_t numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		uint32_t numFeatureLevels = ARRAYSIZE( featureLevels );

		for(uint32_t driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			mDriverType = driverTypes[driverTypeIndex];

			result = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);

			if (result == E_INVALIDARG)
			{
				// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
				result = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &mD3dDevice, &mFeatureLevel, &mImmediateContext);
			}

			if (SUCCEEDED(result))
			{
				break;
			}
		}
		if (FAILED(result))
		{
			return eResult::CAVE_FAIL;
		}

		return eResult::CAVE_OK;
	}

	eResult WindowsDeviceResources::CreateWindowResources(Window* window)
	{
		assert(window != nullptr);
		HWND hWindow = window->GetWindow();
		int32_t result = S_OK;

		// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
		uint32_t numerator = 0u;
		uint32_t denominator = 0u;
		IDXGIFactory1* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			result = static_cast<int32_t>(mD3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)));
			if (SUCCEEDED(static_cast<uint32_t>(result)))
			{
				IDXGIAdapter* adapter = nullptr;
				result = static_cast<int32_t>(dxgiDevice->GetAdapter(&adapter));
				if (SUCCEEDED(static_cast<uint32_t>(result)))
				{
					IDXGIOutput* adapterOutput = nullptr;
					if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
					{
						return eResult::CAVE_FAIL;
					}

					// 출력 (모니터)에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옵니다
					uint32_t numModes = 0u;
					if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
					{
						return eResult::CAVE_FAIL;
					}

					// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성합니다
					DXGI_MODE_DESC* displayModeList = reinterpret_cast<DXGI_MODE_DESC*>(mPool->Allocate(numModes * sizeof(DXGI_MODE_DESC)));
					new(displayModeList) DXGI_MODE_DESC[numModes];
					if (displayModeList == nullptr)
					{
						return eResult::CAVE_FAIL;
					}

					// 이제 디스플레이 모드에 대한 리스트를 채웁니다
					if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
					{
						return eResult::CAVE_FAIL;
					}

					// 이제 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾습니다.
					// 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장합니다.
					for (uint32_t i = 0u; i < numModes; ++i)
					{
						if (displayModeList[i].Width == window->GetWidth())
						{
							if (displayModeList[i].Height == window->GetHeight())
							{
								numerator = displayModeList[i].RefreshRate.Numerator;
								denominator = displayModeList[i].RefreshRate.Denominator;
							}
						}
					}

					// 비디오카드의 구조체를 얻습니다
					DXGI_ADAPTER_DESC adapterDesc;
					if (FAILED(adapter->GetDesc(&adapterDesc)))
					{
						return eResult::CAVE_FAIL;
					}

					// 비디오카드 메모리 용량 단위를 메가바이트 단위로 저장합니다
					mVideoCardMemory = static_cast<int32_t>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

					// 비디오카드의 이름을 저장합니다
					size_t stringLength = 0;
					if (wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128) != 0)
					{
						return eResult::CAVE_FAIL;
					}
					mPool->Deallocate(displayModeList, numModes * sizeof(displayModeList));
					displayModeList = nullptr;

					// 출력 어뎁터를 해제합니다
					adapterOutput->Release();
					adapterOutput = nullptr;


					result = static_cast<int32_t>(adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory)));
					adapter->Release();
					adapter = nullptr;
				}
				dxgiDevice->Release();
			}
		}
		if (FAILED(result))
		{
			return eResult::CAVE_FAIL;
		}

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
		desc.Windowed = TRUE;
		desc.BufferCount = 2;
		desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count = 1;      //multisampling setting
		desc.SampleDesc.Quality = 0;    //vendor-specific flag
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		desc.OutputWindow = hWindow;

		RECT rc;
		GetClientRect(hWindow, &rc);
		uint32_t width = rc.right - rc.left;
		uint32_t height = rc.bottom - rc.top;

		// Create swap chain
		IDXGIFactory2* dxgiFactory2 = nullptr;
		result = dxgiFactory->QueryInterface( __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
		if ( dxgiFactory2 )
		{
			// DirectX 11.1 or later
			result = mD3dDevice->QueryInterface( __uuidof(ID3D11Device1), reinterpret_cast<void**>(&mD3dDevice1));
			if (SUCCEEDED(result))
			{
				static_cast<void>(mImmediateContext->QueryInterface( __uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mImmediateContext1) ));
			}

			DXGI_SWAP_CHAIN_DESC1 sd = {};
			sd.Width = width;
			sd.Height = height;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = 1;

			result = dxgiFactory2->CreateSwapChainForHwnd(mD3dDevice, hWindow, &sd, nullptr, nullptr, &mSwapChain1);
			if (SUCCEEDED(result))
			{
				result = mSwapChain1->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&mSwapChain) );
			}

			dxgiFactory2->Release();
		}
		else
		{
			// DirectX 11.0 systems
			DXGI_SWAP_CHAIN_DESC sd = {};
			sd.BufferCount = 1;
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			// 스캔 라인 순서 및 크기를 지정하지 않음으로 설정합니다.
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// 추가 옵션 플래그를 사용하지 않습니다
			sd.Flags = 0;
			if (mbVsyncEnabled)
			{
				sd.BufferDesc.RefreshRate.Numerator = numerator;
				sd.BufferDesc.RefreshRate.Denominator = denominator;
			}
			else
			{
				sd.BufferDesc.RefreshRate.Numerator = 0u;
				sd.BufferDesc.RefreshRate.Denominator = 1u;
			}
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hWindow;
			sd.SampleDesc.Count = 1;	// multisampling setting off
			sd.SampleDesc.Quality = 0;	// vendor-specific flag
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			// // 출력된 다음 백버퍼를 비우도록 지정합니다
			// sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			if (window->IsFullScreen())
			{
				sd.Windowed = FALSE;
			}
			else
			{
				sd.Windowed = TRUE;
			}

			result = dxgiFactory->CreateSwapChain( mD3dDevice, &sd, &mSwapChain );
		}

		dxgiFactory->Release();

		if (FAILED(result))
		{
			return eResult::CAVE_FAIL;
		}

		result = ConfigureBackBuffer();

		return eResult::CAVE_OK;
	}

	int32_t WindowsDeviceResources::ConfigureBackBuffer()
	{
		int32_t result = S_OK;

		// Create a render target view
		result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mBackBuffer));
		if (FAILED(result))
		{
			return result;
		}
		mBackBuffer->GetDesc(&mBackBufferDesc);

		result = mD3dDevice->CreateRenderTargetView(mBackBuffer, nullptr, &mRenderTargetView);
		if(FAILED(result))
		{
			return result;
		}

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = mBackBufferDesc.Width;
		descDepth.Height = mBackBufferDesc.Height;
		descDepth.MipLevels = 1;	// Use a single mimap level
		descDepth.ArraySize = 1;	// this depth stencil view has only one texture
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		result = mD3dDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencil);
		if (FAILED(result))
		{
			return result;
		}

		// 스텐실 상태 구조체를 초기화합니다
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// 스텐실 상태 구조체를 작성합니다
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// 픽셀 정면의 스텐실 설정입니다
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// 픽셀 뒷면의 스텐실 설정입니다
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// 깊이 스텐실 상태를 생성합니다
		result = mD3dDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
		if (FAILED(result))
		{
			return result;
		}

		// 깊이 스텐실 상태를 설정합니다
		//m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
		mImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		result = mD3dDevice->CreateDepthStencilView(mDepthStencil, &descDSV, &mDepthStencilView);
		if (FAILED(result))
		{
			return result;
		}

		// 렌더링 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩합니다
		mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		// 그려지는 폴리곤과 방법을 결정할 래스터 구조체를 설정합니다
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// 방금 작성한 구조체에서 래스터 라이저 상태를 만듭니다
		if (FAILED(mD3dDevice->CreateRasterizerState(&rasterDesc, &mRasterizerState)))
		{
			return false;
		}

		mImmediateContext->RSSetState(mRasterizerState);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(mBackBufferDesc.Width);
		vp.Height = static_cast<float>(mBackBufferDesc.Height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		mImmediateContext->RSSetViewports(1, &vp);

		// 투영 행렬을 설정합니다
		float fieldOfView = DirectX::XM_PI / 4.0f;
		float screenAspect = static_cast<float>(mBackBufferDesc.Width) / static_cast<float>(mBackBufferDesc.Height);

		// 3D 렌더링을위한 투영 행렬을 만듭니다
		mProjection = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, mWindow->GetNear(), mWindow->GetFar());

		// 세계 행렬을 항등 행렬로 초기화합니다
		mWorld = DirectX::XMMatrixIdentity();

		// 2D 렌더링을위한 직교 투영 행렬을 만듭니다
		mOrtho = DirectX::XMMatrixOrthographicLH(static_cast<float>(mBackBufferDesc.Width), static_cast<float>(mBackBufferDesc.Height), mWindow->GetNear(), mWindow->GetFar());

		// 이제 2D 렌더링을위한 Z 버퍼를 끄는 두 번째 깊이 스텐실 상태를 만듭니다. 유일한 차이점은
		// DepthEnable을 false로 설정하면 다른 모든 매개 변수는 다른 깊이 스텐실 상태와 동일합니다.
		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// 장치를 사용하여 상태를 만듭니다.
		if (FAILED(mD3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &mDepthDisabledStencilState)))
		{
			return false;
		}

		return result;
	}

	int32_t WindowsDeviceResources::ReleaseBackBuffer()
	{
		int32_t result = S_OK;

		// Release the render target view based on the back buffer:
		if (mRenderTargetView != nullptr)
		{
			mRenderTargetView->Release();
			mRenderTargetView = nullptr;
		}

		// Release the back buffer itself:
		if (mBackBuffer != nullptr)
		{
			mBackBuffer->Release();
			mBackBuffer = nullptr;
		}

		// The depth stencil will need to be resized, so release it (and view):
		if (mDepthStencilView != nullptr)
		{
			mDepthStencilView->Release();
		}

		if (mDepthDisabledStencilState != nullptr)
		{
			mDepthDisabledStencilState->Release();
			mDepthDisabledStencilState = nullptr;
		}

		if (mDepthStencilState != nullptr)
		{
			mDepthStencilState->Release();
			mDepthStencilState = nullptr;
		}

		if (mDepthStencil != nullptr)
		{
			mDepthStencil->Release();
			mDepthStencil = nullptr;
		}

		// After releasing references to these resources, we need to call Flush() to 
		// ensure that Direct3D also releases any references it might still have to
		// the same resources - such as pipeline bindings.
		if (mImmediateContext != nullptr)
		{
			mImmediateContext->ClearState();
		}

		return result;
	}

	int32_t WindowsDeviceResources::GoFullScreen()
	{
		int32_t result = S_OK;

		result = mSwapChain->SetFullscreenState(TRUE, NULL);

		// Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
		// call ResizeBuffers in order to realize a full-screen mode switch. Otherwise, 
		// your next call to Present will fail.

		// Before calling ResizeBuffers, you have to release all references to the back 
		// buffer device resource.
		ReleaseBackBuffer();

		// Now we can call ResizeBuffers.
		result = mSwapChain->ResizeBuffers(
			0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
			0, 0,                // Width and height of the swap chain. Set to 0 to match the screen resolution.
			DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
			0
		);

		// Then we can recreate the back buffer, depth buffer, and so on.
		result = ConfigureBackBuffer();

		mWindow->SetFullScreen();

		return result;
	}
	
	int32_t WindowsDeviceResources::GoWindowed()
	{
		int32_t result = S_OK;

		result = mSwapChain->SetFullscreenState(FALSE, NULL);

		// Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
		// call ResizeBuffers in order to realize a change to windowed mode. Otherwise, 
		// your next call to Present will fail.

		// Before calling ResizeBuffers, you have to release all references to the back 
		// buffer device resource.
		ReleaseBackBuffer();

		// Now we can call ResizeBuffers.
		result = mSwapChain->ResizeBuffers(
			0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
			0, 0,                // Width and height of the swap chain. MUST be set to a non-zero value. For example, match the window size.
			DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
			0
		);

		// Then we can recreate the back buffer, depth buffer, and so on.
		result = ConfigureBackBuffer();

		mWindow->SetWindowed();

		return result;
	}

	float WindowsDeviceResources::GetAspectRatio()
	{
		return static_cast<float>(mBackBufferDesc.Width) / static_cast<float>(mBackBufferDesc.Height);
	}

	uint32_t WindowsDeviceResources::GetWidth() const
	{
		return mBackBufferDesc.Width;
	}

	uint32_t WindowsDeviceResources::GetHeight() const
	{
		return mBackBufferDesc.Height;
	}

	ID3D11Device* WindowsDeviceResources::GetDevice()
	{
		return mD3dDevice;
	}
	
	ID3D11DeviceContext* WindowsDeviceResources::GetDeviceContext()
	{
		return mImmediateContext;
	}

	ID3D11RenderTargetView* WindowsDeviceResources::GetRenderTarget()
	{
		return mRenderTargetView;
	}

	ID3D11DepthStencilView* WindowsDeviceResources::GetDepthStencil()
	{
		return mDepthStencilView;
	}

	D3D_DRIVER_TYPE WindowsDeviceResources::GetDriverType() const
	{
		return mDriverType;
	}

	void WindowsDeviceResources::RenderStart()
	{
		// ���� layout ����, topology �ٲٴ°�, vertex buffer �ٲٴ°� rendering �Լ� �ȿ� ���� ��찡 ����
		// vertex buffer�� pixel shader �� ���� �ʼ�
		// Clear the back buffer 
		mImmediateContext->ClearRenderTargetView(mRenderTargetView, DirectX::Colors::MidnightBlue);

		//
		// Clear the depth buffer to 1.0 (max depth)
		//
		mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void WindowsDeviceResources::RenderEnd()
	{
		if (mbVsyncEnabled)
		{
			mSwapChain->Present(1, 0);
		}
		else
		{
			mSwapChain->Present(0, 0);
		}
	}

	void WindowsDeviceResources::Destroy()
	{
		if (mSwapChain != nullptr)
		{
			mSwapChain->SetFullscreenState(false, nullptr);
			mWindow->SetWindowed();
		}

		if (mImmediateContext != nullptr)
		{
			mImmediateContext->ClearState();
		}

		if (mDepthDisabledStencilState != nullptr)
		{
			mDepthDisabledStencilState->Release();
			mDepthDisabledStencilState = nullptr;
		}

		if (mRasterizerState != nullptr)
		{
			mRasterizerState->Release();
			mRasterizerState = nullptr;
		}

		if (mDepthStencil != nullptr)
		{
			mDepthStencil->Release();
			mDepthStencil = nullptr;
		}

		if (mDepthStencilView != nullptr)
		{
			mDepthStencilView->Release();
			mDepthStencilView = nullptr;
		}

		if (mDepthStencilState != nullptr)
		{
			mDepthStencilState->Release();
			mDepthStencilState = nullptr;
		}

		if (mBackBuffer != nullptr)
		{
			mBackBuffer->Release();
			mBackBuffer = nullptr;
		}

		if (mRenderTargetView != nullptr)
		{
			mRenderTargetView->Release();
			mRenderTargetView = nullptr;
		}
		
		if (mSwapChain1 != nullptr)
		{
			mSwapChain1->Release();
			mSwapChain1 = nullptr;
		}

		if (mSwapChain != nullptr)
		{
			mSwapChain->Release();
			mSwapChain = nullptr;
		}

		if (mImmediateContext1 != nullptr)
		{
			mImmediateContext1->Release();
			mImmediateContext1 = nullptr;
		}

		if (mImmediateContext)
		{
			mImmediateContext->Release();
			mImmediateContext = nullptr;
		}

		if (mD3dDevice1 != nullptr)
		{
			mD3dDevice1->Release();
			mD3dDevice1 = nullptr;
		}

		if (mD3dDevice != nullptr)
		{
			mD3dDevice->Release();
			mD3dDevice = nullptr;
		}
	}

	void WindowsDeviceResources::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, mVideoCardDescription);
		memory = mVideoCardMemory;
	}


	void WindowsDeviceResources::TurnZBufferOn()
	{
		mImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);
	}


	void WindowsDeviceResources::TurnZBufferOff()
	{
		mImmediateContext->OMSetDepthStencilState(mDepthDisabledStencilState, 1);
	}
} // namespace cave
#endif