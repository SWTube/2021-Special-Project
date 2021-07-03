/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Sprite/GenericSprite.h"

namespace cave
{
	GenericSprite::GenericSprite(Texture* texture, MemoryPool& pool)
		: mPool(&pool)
		, mTexture(reinterpret_cast<Texture*>(mPool->Allocate(sizeof(Texture))))
	{
		assert(texture != nullptr);
		new(mTexture) Texture(*texture);
		mTexture->Init();
		mWidth = mTexture->GetWidth();
		mHeight = mTexture->GetHeight();
		mTextureIndex = mTexture->GetIndex();
	}

	GenericSprite::GenericSprite(const Texture& texture, MemoryPool& pool)
		: mPool(&pool)
		, mTexture(reinterpret_cast<Texture*>(mPool->Allocate(sizeof(Texture))))
	{
		new(mTexture) Texture(texture);
		mWidth = mTexture->GetWidth();
		mHeight = mTexture->GetHeight();
		mTextureIndex = mTexture->GetIndex();
	}

	GenericSprite::GenericSprite(Texture&& texture, MemoryPool& pool)
		: mPool(&pool)
		, mTexture(reinterpret_cast<Texture*>(mPool->Allocate(sizeof(Texture))))
	{
		new(mTexture) Texture(std::move(texture));
		mWidth = mTexture->GetWidth();
		mHeight = mTexture->GetHeight();
		mTextureIndex = mTexture->GetIndex();
	}

	GenericSprite::GenericSprite(const GenericSprite& other)
		: mPool(other.mPool)
		, mWorld(other.mWorld)
		, mTextureIndex(other.mTextureIndex)
		, mWidth(other.mWidth)
		, mHeight(other.mHeight)
	{
		if (other.mTexture != nullptr)
		{
			mTexture = reinterpret_cast<Texture*>(mPool->Allocate(sizeof(Texture)));
			new(mTexture) Texture(*other.mTexture);
		}
	}

	GenericSprite::GenericSprite(GenericSprite&& other)
		: mPool(other.mPool)
		, mWorld(other.mWorld)
		, mTextureIndex(other.mTextureIndex)
		, mTexture(other.mTexture)
		, mWidth(other.mWidth)
		, mHeight(other.mHeight)
	{
		other.mPool = nullptr;
		other.mTexture = nullptr;
	}

	GenericSprite& GenericSprite::operator=(const GenericSprite& other)
	{
		if (this != &other)
		{
			mWorld = other.mWorld;
			mTextureIndex = other.mTextureIndex;

			mTexture->~Texture();
			if (other.mTexture != nullptr)
			{
				new(mTexture) Texture(*other.mTexture);
			}
			else
			{
				mPool->Deallocate(mTexture, sizeof(Texture));
				mTexture = nullptr;
			}

			mWidth = other.mWidth;
			mHeight = other.mHeight;
		}

		return *this;
	}

	GenericSprite& GenericSprite::operator=(GenericSprite&& other)
	{
		if (this != &other)
		{
			mWorld = other.mWorld;
			mTextureIndex = other.mTextureIndex;

			mTexture = other.mTexture;

			mWidth = other.mWidth;
			mHeight = other.mHeight;

			other.mPool = nullptr;
			other.mTexture = nullptr;
		}

		return *this;
	}

	GenericSprite::~GenericSprite()
	{
		Destroy();
	}

	MemoryPool* GenericSprite::GetMemoryPool() const
	{
		return mPool;
	}

#ifdef __WIN32__
	eResult GenericSprite::Init(ID3D11Device* device, ID3D11DeviceContext* context, uint32_t screenWidth, uint32_t screenHeight)
#else
	eResult GenericSprite::Init(uint32_t program, uint32_t screenWidth, uint32_t screenHeight)
#endif
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		mPreviousPosition = Float3(-1.0f, -1.0f, 1.0f);
		mPosition = Float3(0.0f, 0.0f, 1.0f);

#ifdef __WIN32__
		eResult result = initializeBuffers(device, context);
#else
		eResult result = initializeBuffers(program);
#endif

		return result;
	}

	void GenericSprite::Destroy()
	{
		if (mTexture != nullptr)
		{
			mTexture->DeleteTexture();
			mTexture->~Texture();
			mPool->Deallocate(mTexture, sizeof(Texture));
			mTexture = nullptr;
		}
	}

	void GenericSprite::SetTexture(const Texture& texture)
	{
		if (mTexture != nullptr)
		{
			mTexture->~Texture();
			mTexture = nullptr;
		}
		*mTexture = texture;
	}
} // namespace cave
