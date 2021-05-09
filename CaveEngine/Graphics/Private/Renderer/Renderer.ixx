/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

module;

#include "Graphics.h"

export module Renderer;

namespace cave
{
	namespace Renderer
	{
		//--------------------------------------------------------------------------------------
		// Structures
		//--------------------------------------------------------------------------------------
		struct SimpleVertex
		{
			DirectX::XMFLOAT3 Pos;
		};

		//--------------------------------------------------------------------------------------
		// Global Variables
		//--------------------------------------------------------------------------------------
		HINSTANCE gHInstance = nullptr;
		HWND gHWindow = nullptr;
		D3D_DRIVER_TYPE gDriverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL gFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11Device* gD3dDevice = nullptr;
		ID3D11Device1* gD3dDevice1 = nullptr;
		ID3D11DeviceContext* gImmediateContext = nullptr;
		ID3D11DeviceContext1* gImmediateContext1 = nullptr;
		IDXGISwapChain* gSwapChain = nullptr;
		IDXGISwapChain1* gSwapChain1 = nullptr;
		ID3D11RenderTargetView* gRenderTargetView = nullptr;
		ID3D11VertexShader* gVertexShader = nullptr;
		ID3D11PixelShader* gPixelShader = nullptr;
		ID3D11InputLayout* gVertexLayout = nullptr;
		ID3D11Buffer* gVertexBuffer = nullptr;
		// 텍스처 사용하지 않으므로 shader resource view는 사용하지 않음

		//--------------------------------------------------------------------------------------
		// Forward declarations
		//--------------------------------------------------------------------------------------
		void CleanupDevice();
		HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		void Destroy();
		HRESULT Init(HINSTANCE hInstance, int nCmdShow, LPCWSTR className, LPCWSTR windowName);
		HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR className, LPCWSTR windowName);
		HRESULT InitDevice();
		void Render();
		LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

		//--------------------------------------------------------------------------------------
		// Clean up the objects we've created
		//--------------------------------------------------------------------------------------
		void CleanupDevice()
		{
			if (gImmediateContext)
			{
				gImmediateContext->ClearState();
			}

			if (gVertexBuffer)
			{
				gVertexBuffer->Release();
			}
			if (gVertexLayout)
			{
				gVertexLayout->Release();
			}
			if (gVertexShader)
			{
				gVertexShader->Release();
			}
			if (gPixelShader)
			{
				gPixelShader->Release();
			}
			if (gRenderTargetView)
			{
				gRenderTargetView->Release();
			}
			if (gSwapChain1)
			{
				gSwapChain1->Release();
			}
			if (gSwapChain)
			{
				gSwapChain->Release();
			}
			if (gImmediateContext1)
			{
				gImmediateContext1->Release();
			}
			if (gImmediateContext)
			{
				gImmediateContext->Release();
			}
			if (gD3dDevice1)
			{
				gD3dDevice1->Release();
			}
			if (gD3dDevice)
			{
				gD3dDevice->Release();
			}
		}

		//--------------------------------------------------------------------------------------
		// Helper for compiling shaders with D3DCompile
		//
		// With VS 11, we could load up prebuilt .cso files instead...
		//--------------------------------------------------------------------------------------
		HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
		{
			// szFileName 파일 이름
			// szEntryPoint 이 파일의 어떤 함수 컴파일할거다
			// szShaderModel 쉐이더 모델
			HRESULT hResult = S_OK;

			// 컴파일 옵션
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
			// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
			// Setting this flag improves the shader debugging experience, but still allows 
			// the shaders to be optimized and to run exactly the way they will run in 
			// the release configuration of this program.
			dwShaderFlags |= D3DCOMPILE_DEBUG;
			// 이거 들어가면 느려짐. 쉐이더 디버깅할 때 쓰면 됨. 웬만하면 하기 싫지만 정말 마지막의 마지막에 써야할 때도

			// Disable optimizations to further improve shader debugging
			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			// ID3DBlob 텍스트 형태의 무언가를 저장할 때. 쉐이더 말고는 쓸 곳이 있나...?
			// 미리 쉐이더 컴파일 해두고 런타임엔 쉐이더 바이너리를 바로 Blob에 가져오면 컴파일 없이 사용할 수 있음
			// 대부분의 경우 쉐이더는 옵션에 따라 컴파일 다르게 해야할 수도 있음
			// 이럴 때 ifdef을 컴파일 당시에 넣어줘서 다르게 해줄 수도
			ID3DBlob* pErrorBlob = nullptr;

			// GLUT처럼 D3DX라는 헬퍼 라이브러리가 있음. 12부터 사라짐
			// MS도 어느 순간 11에서 D3DX 권장 안함
			// d3dx들은 prod 레벨에서 없애야 12 포팅할 때 쉬움
			// 여기 두번째 매개변수가 PDefines인데 여기에 ifdef에 해당하는 걸 다 넣어버릴 수 있음
			// 쉐이더 파일은 같아도 컴파일 때 def에 따라 다른 바이너리 겟 가능
			// 매번 컴파일 다시하면 느려지니까 쉐이더 캐시 사용
			// 겜 업뎃할 때 쉐이더 업뎃될 수도 있으니, 이전 쉐이더 캐시랑 비교해서, 
			// 사이즈 / 내용물 바뀌었으면 컴파일해서 바이너리를 캐쉬 쪽에 갱신
			// 쉐이더 캐시 바이너리로부터 쉐이더 생성
			// 변경 사항 없으면 그냥 캐시에서 바로 읽어서 생성
			hResult = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
				dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
			if (FAILED(hResult))
			{
				if (pErrorBlob)
				{
					OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
					pErrorBlob->Release();
				}
				return hResult;
			}
			if (pErrorBlob)
			{
				pErrorBlob->Release();
			}

			return S_OK;
		}

		//--------------------------------------------------------------------------------------
		// Destroy Renderer
		//--------------------------------------------------------------------------------------
		export void Destroy()
		{
			CleanupDevice();
		}

		//--------------------------------------------------------------------------------------
		// Initialize Renderer
		//--------------------------------------------------------------------------------------
		export HRESULT Init(HINSTANCE hInstance, int nCmdShow, LPCWSTR className, LPCWSTR windowName)
		{
			HRESULT hResult = FAILED(InitWindow(hInstance, nCmdShow, className, windowName));
			if (FAILED(hResult))
			{
				return hResult;
			}

			hResult = InitDevice();
			if (FAILED(hResult))
			{
				Destroy();
				return hResult;
			}

			return hResult;
		}

		//--------------------------------------------------------------------------------------
		// Create Direct3D device and swap chain
		//--------------------------------------------------------------------------------------
		HRESULT InitDevice()
		{
			HRESULT hResult = S_OK;

			RECT rect;    // 화면 크기
			GetClientRect(gHWindow, &rect);
			UINT width = rect.right - rect.left;
			UINT height = rect.bottom - rect.top;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_DRIVER_TYPE driverTypes[] = {
				D3D_DRIVER_TYPE_HARDWARE,   // GPU로
				D3D_DRIVER_TYPE_WARP,       // CPU로
				D3D_DRIVER_TYPE_REFERENCE,  // 솦트로 GPU 돌리는 거임
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0, // 더 넣어도 되긴 하는데, 최우선으로 원하는 것 순서로 되어있는 것. 되냐 여부는 GPU가 결정
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				gDriverType = driverTypes[driverTypeIndex];
				hResult = D3D11CreateDevice(nullptr, gDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
					D3D11_SDK_VERSION, &gD3dDevice, &gFeatureLevel, &gImmediateContext);

				if (hResult == E_INVALIDARG)
				{
					// DirectX 11.0 platforg will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
					hResult = D3D11CreateDevice(nullptr, gDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
						D3D11_SDK_VERSION, &gD3dDevice, &gFeatureLevel, &gImmediateContext);
					// 성공한 featureLevel를 gFeatureLevel에 전달해줌
				}

				if (SUCCEEDED(hResult))
				{
					break;
				}
			}
			if (FAILED(hResult))
			{
				return hResult;
			}

			// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
			IDXGIFactory1* dxgiFactory = nullptr;
			{
				IDXGIDevice* dxgiDevice = nullptr;
				hResult = gD3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
				if (SUCCEEDED(hResult))
				{
					IDXGIAdapter* adapter = nullptr;
					hResult = dxgiDevice->GetAdapter(&adapter);
					if (SUCCEEDED(hResult))
					{
						hResult = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
						adapter->Release();
					}
					dxgiDevice->Release();
				}
			}
			if (FAILED(hResult))
			{
				return hResult;
			}

			// Create swap chain
			IDXGIFactory2* dxgiFactory2 = nullptr;
			hResult = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
			if (dxgiFactory2)
			{
				// DirectX 11.1 or later
				hResult = gD3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&gD3dDevice1));
				if (SUCCEEDED(hResult))
				{
					static_cast<void>(gImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&gImmediateContext1)));
				}

				DXGI_SWAP_CHAIN_DESC1 sd = {};
				sd.Width = width;
				sd.Height = height;
				sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
				sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				sd.BufferCount = 1;

				hResult = dxgiFactory2->CreateSwapChainForHwnd(gD3dDevice, gHWindow, &sd, nullptr, nullptr, &gSwapChain1);
				if (SUCCEEDED(hResult))
				{
					hResult = gSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&gSwapChain));
				}

				dxgiFactory2->Release();
			}
			else
			{
				// DirectX 11.0 systeg
				DXGI_SWAP_CHAIN_DESC sd = {};   // w buffering. flip / bullet
												// flip: front buffer와 back buffer를 순간적으로 바꿈
												// bullet: back buffer를 front buffer로 쏴줌
				sd.BufferCount = 1; // 이거로 더블 버퍼링 / 트리플 버퍼링 조절 가능
				sd.BufferDesc.Width = width;
				sd.BufferDesc.Height = height;
				sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // UNORM -> 0.0 ~ 1.0
				sd.BufferDesc.RefreshRate.Numerator = 60;   // 60 fps
				sd.BufferDesc.RefreshRate.Denominator = 1;
				sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				sd.OutputWindow = gHWindow;
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
				sd.Windowed = TRUE; // window 모드냐?

				hResult = dxgiFactory->CreateSwapChain(gD3dDevice, &sd, &gSwapChain);
			}

			// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
			dxgiFactory->MakeWindowAssociation(gHWindow, DXGI_MWA_NO_ALT_ENTER);

			dxgiFactory->Release();

			if (FAILED(hResult))
			{
				return hResult;
			}

			// Create a render target view
			// view라는 것은 원래 리소스가 어딘가 존재하고, 그 리소스를 사용하기 위한 파생 인터페이스. 원래는 read-only
			// shader resource view면 shader 안에서 사용할 resource인데, 그 근원은 텍스처. 이거 쓸 수도 / 읽을 수도 있음
			// 쓸 땐 shader resource view가 아님
			// 텍스처를 읽어서 사용하려면, 해당 텍스처를 가리키는 포인터를 갖는 인터페이스를 생성해서 던져주기 (shader resource view)
			// 화면 버퍼도 텍스처 리소스. 여기에 write를 할거면 render target view가 되어야함
			ID3D11Texture2D* pBackBuffer = nullptr;
			hResult = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
			// swap chain이 갖고 있는 백 버퍼 만들기
			if (FAILED(hResult))
			{
				return hResult;
			}

			// ID3D11Device         몸체
				// 생성 / 해제하는 기능
			// ID3D11DeviceContext  흐름
				// 그리기 / 상태 바꾸기. 몸체보다 상위 개념
				// 현재 상태에서 텍스처 세팅, 드로잉, 화면 클리어 등이라고 할 때의 "현재 상태"가 context
				// opengl은 암묵적으로 따로 지정안하고 현재 상태는 하나라고 가정하고 전역으로 호출
				// 12부터는 멀티 스레드 렌더링 지원해서 개념상으로 context 구분. context 여러 개가 될 수 있다
				// 물론 12라고 해서 실제 context를 여러 개 사용하지는 않음. 여러 개가 있는데 그 중 하나를 주로 사용하는 것.
					// 그게 immediate context
			// ID3D11RenderTargetView 화면에 write 되는 버퍼
				// 쉐이더를 통해 화면에 그리고, 명시적으로 화면 클리어할 때 사용
			hResult = gD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &gRenderTargetView);
			pBackBuffer->Release(); // 이걸 왜 해주지?
									// gRenderTargetView가 pBackBuffer를 참조해서 ref count가 1 증가해버리기 때문
									// 이제 pBackBuffer 쓸 일 없을테니까...
			if (FAILED(hResult))
			{
				return hResult;
			}

			// 여러 target 세팅 가능. 요즘은 8 장까지 가능
			// 마지막 parameter는 depth buffer. 안쓰니까 nullptr
			// 앞에 OM 붙어있다는 건 Output Manager. 버퍼에 쓰고 뭐하고 하는 짓은 접두어가 OM으로 되어있음
			// 우리가 그릴 buffer 세팅
			gImmediateContext->OMSetRenderTargets(1, &gRenderTargetView, nullptr);

			// gRenderTargetView의 버퍼 중 어디까지를 그릴 건지?
			// viewport로 3ds max 만든다고 생각하면 화면 4분할해서
			// 각각 렌더링하면 버퍼 하나에 viewport의 값을 4분할한 값만큼 넣어서 
			// 렌더링할 때 각각 RSSetViewports해주면 됨
			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = static_cast<FLOAT>(width);
			vp.Height = static_cast<FLOAT>(height);
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			// Rasterizer Stage
			gImmediateContext->RSSetViewports(1, &vp);

			// 화면을 그릴 버퍼와 버퍼 영역 설정 완료

			// 쉐이더 컴파일
			// 9 -> 10 넘어갈 때 iPhone 출시되면서 모바일 환경이 핫해지면서 9 -> 11로 안넘어감
			// 그 이유가 shader. DX9까지는 쉐이더가 옵션이었는데 이후엔 쉐이더가 필수가 됨
			// 그전까지는 fvf로 세팅해서 고정 파이프라인 사용 (쉐이더 안 쓰고)
			// 9.3.9.0c 때부터 GPU의 발전으로 쉐이더 떡칠 시작
			// 고정 파이프라인 없어짐. 쉐이더 하나도 없으면 렌더링 안됨
			// Compile the vertex shader
			ID3DBlob* pVSBlob = nullptr;
			// shader 모델은 vs 4.0이구나~
			// 4.0은 사실 10 버전이고 11은 5.0버전인데 어차피 다 호환 됨
			// 전역이니까 directx device가 들고 있는 건 아니겠구나~
			// 이거 걍 헬퍼 함수임
			hResult = CompileShaderFromFile(L"Graphics/Shader/DirectXTest.fxh", "VS", "vs_4_0", &pVSBlob);
			if (FAILED(hResult))
			{
				MessageBox(nullptr,
					L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
				return hResult;
			}

			// Create the vertex shader
			// 생성은 몸체에서. Blob에서 get하기
			// 오류 나면 보통 여기 말고 위에 Compile Shader에서 걸림
			// 쉐이더 코드가 멀쩡한데 글픽카드가 feature level를 10.0 즈음 지원하는데
			// 쉐이더 모드를 5.0로 짜면 컴파일만 되고 바이너리로 쉐이더 생성이 안됨

			// 윈도우 함수가 성공했는지 여부 보려면 watch에서 @err, hr하면 현재 스레드에서 윈도우 함수가 최종 리턴한 거 볼 수 있음
			hResult = gD3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &gVertexShader);
			if (FAILED(hResult))
			{
				pVSBlob->Release();
				return hResult;
			}

			// layout은 GPU에 전달할 버텍스가 어떤 타입인지, 구성요소가 어떻게 되는지 알려주는 것
			// DirectXTest.fxh 코드 참고해보면
			// Vulkan이나 Metal, DX11 이상은 struct가 들어감.
			// 이 예제처럼 simple한 Shader의 경우 16byte 짜리 float 변수 하나만
			// hlsl / glsl 제일 큰 부분은 앞에 struct 선언하는 부분
			// glsl는 uniform으로 string으로 이름 지정
			// hlsl는 구조체 단위로 작성
			// "POSITION"은 그냥 D3D Semantic임
			// 32bit RGB Float. 입력 단위 자체는 언제나 float4. 16 byte 정렬되어있어야
			// layout은 여러 개가 가능함. position 좌표, 텍스처 좌표 등 구분해서
			// layout은 유형마다 서로 다르게 갖고 있을 수도 있음
			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			// gVertexLayout. ID3D11InputLayout 개체. COM 개체임.
			// Create the input layout
			hResult = gD3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
				pVSBlob->GetBufferSize(), &gVertexLayout);
			pVSBlob->Release();
			if (FAILED(hResult))
			{
				return hResult;
			}

			// IA. Input Assembly
			// Set the input layout
			gImmediateContext->IASetInputLayout(gVertexLayout);

			// Compile the pixel shader
			ID3DBlob* pPSBlob = nullptr;
			hResult = CompileShaderFromFile(L"Graphics/Shader/DirectXTest.fxh", "PS", "ps_4_0", &pPSBlob);
			if (FAILED(hResult))
			{
				MessageBox(nullptr,
					L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
				return hResult;
			}

			// Create the pixel shader
			hResult = gD3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &gPixelShader);
			pPSBlob->Release();
			if (FAILED(hResult))
			{
				return hResult;
			}

			// XMFLOAT3 32bit
			// LHS. x 가로(너비) y 세로(높이) z 깊이
			// 이 예제에서는 화면 버퍼에서 바로 쓸 수 있게 x, y는 -1.0 ~ 1.0, z는 0.0 ~ 1.0으로 넣어주는 것
			// Matrix 변환 하나도 없음.
			// Create vertex buffer
			SimpleVertex vertices[] = {
				DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f),
				DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
				DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),
			};
			D3D11_BUFFER_DESC bd = {};
			// IMMUTABLE: 생성할 당시 그 버퍼에 값을 포인터로 전달한 다음 절대 건드리지 않기
			// DYNAMIC: 쓸 때마다 락 걸어서 사용. 보통은 DEFAULT 많이 사용. 최대한 GPU 메모리 안에 리소스 위치. DYNAMIC 쓰면 
			// 이 내용이 변경 될 수 있다는 뜻. 시스템 메모리에 썻다 GPU 메모리에 내부적으로 전달하는 형태
			// STAGING은 GPU 메모리 아님. 절대 GPU에 가지 않음. DX 리소스 다루는 방법으로 copy할 때 호환성 유지할 때
				// GPU 메모리 안에 어떤 값을 알고 싶으면 화면 버퍼의 데이터를 staging으로 copy해서 읽어오기.
				// GPU 메모리를 바로 못 읽음
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * 3;
			// 버퍼 메모리 유형
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			// 텍스처
			D3D11_SUBRESOURCE_DATA InitData = {};
			InitData.pSysMem = vertices;
			hResult = gD3dDevice->CreateBuffer(&bd, &InitData, &gVertexBuffer);
			if (FAILED(hResult))
			{
				return hResult;
			}

			// 현재 context에 묶어주기
			// Set vertex buffer
			UINT stride = sizeof(SimpleVertex); // 12 byte
			UINT offset = 0;
			gImmediateContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &stride, &offset);

			// Set primitive topology
			gImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			return S_OK;
		}

		//--------------------------------------------------------------------------------------
		// Register class and create window
		//--------------------------------------------------------------------------------------
		HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, LPCWSTR className, LPCWSTR windowName)
		{
			// Register class
			WNDCLASSEX windowClassEx;
			windowClassEx.cbSize = sizeof(WNDCLASSEX);
			windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
			windowClassEx.lpfnWndProc = WndProc;
			windowClassEx.cbClsExtra = 0;
			windowClassEx.cbWndExtra = 0;
			windowClassEx.hInstance = hInstance;
			windowClassEx.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(IDI_TUTORIAL1));
			windowClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
			windowClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
			windowClassEx.lpszMenuName = nullptr;
			windowClassEx.lpszClassName = className;
			windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, reinterpret_cast<LPCTSTR>(IDI_TUTORIAL1));
			if (!RegisterClassEx(&windowClassEx))
			{
				return E_FAIL;
			}

			// Create window
			gHInstance = hInstance;
			RECT rect = {0, 0, 800, 600};
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			gHWindow = CreateWindow(className, windowName,
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance,
				nullptr);
			if (!gHWindow)
			{
				return E_FAIL;
			}

			ShowWindow(gHWindow, nCmdShow);

			return S_OK;
		}

		//--------------------------------------------------------------------------------------
		// Render a frame
		//--------------------------------------------------------------------------------------
		export void Render()
		{
			// 원래 layout 세팅, topology 바꾸는거, vertex buffer 바꾸는거 rendering 함수 안에 들어가는 경우가 보통
			// vertex buffer랑 pixel shader 두 개는 필수
			// Clear the back buffer 
			gImmediateContext->ClearRenderTargetView(gRenderTargetView, DirectX::Colors::MidnightBlue);

			// Render a triangle
			gImmediateContext->VSSetShader(gVertexShader, nullptr, 0);
			gImmediateContext->PSSetShader(gPixelShader, nullptr, 0);
			// 실제 production level에선 Draw보다는 DrawIndexed를 더 많이 사용
			// 첫번째는 버텍스 개수. 3
			// back buffer에 그리기
			gImmediateContext->Draw(3, 0);

			// 이제 flip이나 swap 통해서 front buffer에 쏴줌
			// front buffer에 보내기
			// 실제 그려지는 순간은 GPU Driver마다 다름. 요즘은 내부적으로 비동기로 일어남
			// Present the information rendered to the back buffer to the front buffer (the screen)
			gSwapChain->Present(0, 0);
		}

		//--------------------------------------------------------------------------------------
		// Called every time the application receives a message
		//--------------------------------------------------------------------------------------
		LRESULT CALLBACK WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
		{
			PAINTSTRUCT paintStruct;
			HDC handleDeviceContext;

			switch (message)
			{
			case WM_PAINT:
				handleDeviceContext = BeginPaint(hWindow, &paintStruct);
				EndPaint(hWindow, &paintStruct);
				break;

			case WM_DESTROY:
				PostQuitMessage(0);
				break;

				// Note that this tutorial does not handle resizing (WM_SIZE) requests,
				// so we created the window without the resize border.

			default:
				return DefWindowProc(hWindow, message, wParam, lParam);
			}

			return 0;
		}
	}
}