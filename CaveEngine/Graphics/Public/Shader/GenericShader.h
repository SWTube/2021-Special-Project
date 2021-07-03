/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <filesystem>

#include "GraphicsApiPch.h"

#include "CoreGlobals.h"
#include "CoreTypes.h"
#include "Memory/MemoryPool.h"
#include "Sprite/Sprite.h"

namespace cave
{
	class GenericShader
	{
	public:
		GenericShader() = delete;
		GenericShader(const std::filesystem::path& vertexShaderFilePath, const std::filesystem::path& fragmentShaderFilePath, MemoryPool& pool);
		GenericShader(const std::filesystem::path& shaderFilePath, MemoryPool& pool);
		GenericShader(const GenericShader&) = delete;
		GenericShader(GenericShader&& other);
		GenericShader& operator=(const GenericShader&) = delete;
		GenericShader& operator=(GenericShader&& other);
		virtual ~GenericShader();
		constexpr MemoryPool* GetMemoryPool() const;
		
#ifdef __WIN32__
		virtual eResult Compile(ID3D11Device* device) = 0;
		virtual void Render(ID3D11DeviceContext* context, uint32_t indexCount, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture) = 0;
#else
		virtual eResult Compile() = 0;
		virtual void Render(uint32_t indexCount, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, uint32_t texture) = 0;
#endif
#ifdef __WIN32__
		virtual eResult SetInputLayout() = 0;
#else
		virtual eResult SetInputLayout(const Sprite& sprite) = 0;
#endif
		virtual void Destroy();
	protected:
		struct Buffer
		{
#ifdef __WIN32__
			DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX View = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX Projection = DirectX::XMMatrixIdentity();
#else
			glm::mat4 World = glm::mat4(1.0f);
			glm::mat4 View = glm::mat4(1.0f);
			glm::mat4 Projection = glm::mat4(1.0f);
#endif
		};
		MemoryPool* mPool = nullptr;
		std::filesystem::path mShaderFilePath = PROJECT_DIR;
		std::filesystem::path mVertexShaderFilePath = PROJECT_DIR;
		std::filesystem::path mFragmentShaderFilePath = PROJECT_DIR;
#ifdef __WIN32__
		ID3D11Buffer* mBuffer = nullptr;
#endif
	};
} // namespace cave
