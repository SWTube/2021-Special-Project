/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"
#include "Sprite/Vertex.h"
#include "Texture/Texture.h"

namespace cave
{
	class GenericSprite
	{
	public:
		GenericSprite() = delete;
		GenericSprite(Texture* texture, MemoryPool& pool);
		GenericSprite(const Texture& texture, MemoryPool& pool);
		GenericSprite(Texture&& texture, MemoryPool& pool);
		GenericSprite(const GenericSprite& other);
		GenericSprite(GenericSprite&& other);
		virtual ~GenericSprite();
		GenericSprite& operator=(const GenericSprite& other);
		GenericSprite& operator=(GenericSprite&& other);
		MemoryPool* GetMemoryPool() const;

#ifdef __WIN32__
		virtual eResult Init(ID3D11Device* device, ID3D11DeviceContext* context, uint32_t screenWidth, uint32_t screenHeight);
#else
		virtual eResult Init(uint32_t program, uint32_t screenWidth, uint32_t screenHeight);
#endif
		virtual void Destroy();
		virtual void Update() = 0;
		virtual void Render() = 0;

		constexpr uint32_t GetIndicesCount() const;
		constexpr const Texture* GetTexture() const;
		constexpr void SetPosition(float x, float y);
		constexpr void SetPosition(const Float2& position);
		constexpr void SetPosition(const Float2&& position);
		constexpr void Move(float x, float y);
		constexpr float GetPositionX() const;
		constexpr float GetPositionY() const;
		constexpr Float2 GetPosition() const;
		constexpr void SetTextureIndex(uint32_t index);
		constexpr uint32_t GetTextureIndex();
		constexpr uint32_t GetWidth() const;
		constexpr uint32_t GetHeight() const;
		constexpr void GetSize(uint32_t& outWidth, uint32_t& outHeight) const;
		constexpr void SetSize(uint32_t width, uint32_t height);
		virtual void SetTexture(const Texture& texture);

	protected:
#ifdef __WIN32__
		virtual eResult initializeBuffers(ID3D11Device* device, ID3D11DeviceContext* context) = 0;
#else
		virtual eResult initializeBuffers(uint32_t program) = 0;
#endif
		static constexpr uint32_t VERTICES_COUNT = 4u;
		static constexpr uint32_t INDICES_COUNT = 6u;

		VertexT mVertices[VERTICES_COUNT] = {
			VertexT(-1.0f,  1.0f, 0.0f,	1.0f, 1.0f),	// top left
			VertexT( 1.0f,  1.0f, 0.0f,	0.0f, 1.0f),	// top right
			VertexT( 1.0f, -1.0f, 0.0f,	0.0f, 0.0f),	// bottom right
			VertexT(-1.0f, -1.0f, 0.0f,	1.0f, 0.0f),	// bottom left
		};

		static constexpr uint8_t INDICES[INDICES_COUNT] = {
			0u, 1u, 2u,
			2u, 3u, 0u,
		};

		MemoryPool* mPool = nullptr;
#ifdef __WIN32__
		typedef struct ConstantBuffer
		{
			DirectX::XMMATRIX mWorld;
		} ConstantBuffer;

		// Assert that the constant buffer remains 16-byte aligned.
		static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

		DirectX::XMMATRIX mWorld = DirectX::XMMatrixIdentity();
#else
		glm::mat4 mWorld = glm::mat4(1.0f);
#endif

		uint32_t mTextureIndex = 0u;
		Texture* mTexture = nullptr;

		uint32_t mWidth = 0u;
		uint32_t mHeight = 0u;
		uint32_t mScreenWidth = 0u;
		uint32_t mScreenHeight = 0u;

		Float3 mPosition;
		Float3 mPreviousPosition;

		bool mbNeedsUpdate = false;
	};

	constexpr uint32_t GenericSprite::GetIndicesCount() const
	{
		return INDICES_COUNT;
	}

	constexpr const Texture* GenericSprite::GetTexture() const
	{
		return mTexture;
	}

	constexpr void GenericSprite::SetPosition(float x, float y)
	{
		mPosition.X = x;
		mPosition.Y = y;
	}

	constexpr void GenericSprite::SetPosition(const Float2& position)
	{
		mPosition.X = position.X;
		mPosition.Y = position.Y;
	}

	constexpr void GenericSprite::SetPosition(const Float2&& position)
	{
		mPosition.X = position.X;
		mPosition.Y = position.Y;
	}

	constexpr void GenericSprite::Move(float x, float y)
	{
		if (mPosition.X <= FLT_MAX - x)
		{
			mPosition.X += x;
		}
		else
		{
			mPosition.X = FLT_MAX;
		}

		if (mPosition.Y <= FLT_MAX - y)
		{
			mPosition.Y += y;
		}
		else
		{
			mPosition.Y = FLT_MAX;
		}
	}

	constexpr float GenericSprite::GetPositionX() const
	{
		return mPosition.X;
	}

	constexpr float GenericSprite::GetPositionY() const
	{
		return mPosition.Y;
	}

	constexpr Float2 GenericSprite::GetPosition() const
	{
		return Float2(mPosition.X, mPosition.Y);
	}

	constexpr void GenericSprite::SetTextureIndex(uint32_t index)
	{
		mTextureIndex = index;
	}

	constexpr uint32_t GenericSprite::GetTextureIndex()
	{
		return mTextureIndex;
	}

	constexpr uint32_t GenericSprite::GetWidth() const
	{
		return mWidth;
	}

	constexpr uint32_t GenericSprite::GetHeight() const
	{
		return mHeight;
	}

	constexpr void GenericSprite::GetSize(uint32_t& outWidth, uint32_t& outHeight) const
	{
		outWidth = mWidth;
		outHeight = mHeight;
	}

	constexpr void GenericSprite::SetSize(uint32_t width, uint32_t height)
	{
		mWidth = width;
		mHeight = height;
		mbNeedsUpdate = true;
	}
} // namespace cave
