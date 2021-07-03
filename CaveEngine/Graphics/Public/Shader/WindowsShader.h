/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <filesystem>

#include "GraphicsApiPch.h"

#include "Shader/GenericShader.h"

#ifdef __WIN32__
	namespace cave
	{
		class WindowsShader final : public GenericShader
		{
			struct MatrixBufferType
			{
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX view;
				DirectX::XMMATRIX projection;
			};

		public:
			WindowsShader() = delete;
			WindowsShader(const std::filesystem::path& shaderFilePath, MemoryPool& pool);
			WindowsShader(const std::filesystem::path& vertexShaderFilePath, const std::filesystem::path& fragmentShaderFilePath, MemoryPool& pool);
			WindowsShader(const WindowsShader&) = delete;
			WindowsShader(WindowsShader&& other);
			WindowsShader& operator=(const WindowsShader&) = delete;
			WindowsShader& operator=(WindowsShader&& other);
			virtual ~WindowsShader();

			virtual eResult Compile(ID3D11Device* device) override;
			virtual eResult SetInputLayout() override;
			virtual void Render(ID3D11DeviceContext* context, uint32_t indexCount, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture) override;

			virtual void Destroy() override;
		private:
			eResult compileShaderFromFile(const wchar_t* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);

			ID3D11VertexShader* mVertexShader = nullptr;
			ID3D11PixelShader* mPixelShader = nullptr;
			ID3D11InputLayout* mInputLayout = nullptr;
			ID3D11SamplerState* mSamplerLinear = nullptr;
		};

		typedef WindowsShader Shader;
	} // namespace cave
#endif