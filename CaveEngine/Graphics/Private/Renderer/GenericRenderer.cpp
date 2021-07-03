/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Renderer/GenericRenderer.h"

namespace cave
{

	GenericRenderer::GenericRenderer()
		: mPool(reinterpret_cast<MemoryPool*>(gCoreMemoryPool.Allocate(sizeof(MemoryPool))))
		, mFrameCount(0u)
	{
		new(mPool) MemoryPool(RENDERER_MEMORY_SIZE);
	}

	GenericRenderer::~GenericRenderer()
	{
		Destroy();
	}

	void GenericRenderer::Destroy()
	{
		while (!mTextures.empty())
		{
			Texture* texture = mTextures.back();
			texture->~Texture();
			mPool->Deallocate(texture, sizeof(Sprite));
			texture = nullptr;
			mTextures.pop_back();
		}
		mTextures.clear();
		// std::vector<Texture*>().swap(mTextures);

		while (!mSprites.empty())
		{
			Sprite* sprite = mSprites.back();
			sprite->~Sprite();
			mPool->Deallocate(sprite, sizeof(Sprite));
			sprite = nullptr;
			mSprites.pop_back();
		}
		mSprites.clear();

		if (mShader != nullptr)
		{
			mShader->~Shader();
			mPool->Deallocate(mShader, sizeof(Shader));
			mShader = nullptr;
		}

		if (mCamera != nullptr)
		{
			mCamera->~Camera();
			mPool->Deallocate(mCamera, sizeof(Camera));
			mCamera = nullptr;
		}

		if (mDeviceResources != nullptr)
		{
			mDeviceResources->~DeviceResources();
			mPool->Deallocate(mDeviceResources, sizeof(DeviceResources));
			mDeviceResources = nullptr;
		}

		if (mPool != nullptr)
		{
			mPool->~MemoryPool();
			gCoreMemoryPool.Deallocate(mPool, sizeof(MemoryPool));
			mPool = nullptr;
		}
	}

	DeviceResources* GenericRenderer::GetDeviceResources() const
	{
		return mDeviceResources;
	}

	eResult GenericRenderer::AddSprite(Sprite&& object)
	{
		Sprite* newSprite = reinterpret_cast<Sprite*>(mPool->Allocate(sizeof(Sprite)));
		new(newSprite) Sprite(std::move(object));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(cave::eLogChannel::GRAPHICS, std::cerr, "glCreateBuffers error code: 0x%x", glError);
		}
#ifdef __WIN32__
		newSprite->SetTextureIndex(0u);
		eResult result = newSprite->Init(mDeviceResources->GetDevice(),mDeviceResources->GetDeviceContext(), mDeviceResources->GetWidth(), mDeviceResources->GetHeight());
#else
		eResult result = newSprite->Init(mDeviceResources->GetProgram(), mDeviceResources->GetWidth(), mDeviceResources->GetHeight());
#endif
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

#if !defined(__WIN32__)
		mShader->SetInputLayout(*newSprite);
#endif

		mSprites.push_back(newSprite);

		return result;
	}

	eResult GenericRenderer::AddTexture(Texture&& texture)
	{
		Texture* newTexture = reinterpret_cast<Texture*>(mPool->Allocate(sizeof(Texture)));
		new(newTexture) Texture(std::move(texture));
		mTextures.push_back(newTexture);

		return eResult::CAVE_OK;
	}

	eResult GenericRenderer::RemoveSprite(uint32_t index)
	{
		assert(index < mSprites.size());

		Sprite* sprite = mSprites[index];
		sprite->Destroy();
		mPool->Deallocate(sprite, sizeof(Sprite));
		mSprites.erase(mSprites.begin() + index);

		return eResult::CAVE_OK;
	}

	eResult GenericRenderer::RemoveTexture(uint32_t index)
	{
		assert(index < mTextures.size());

		Texture* texture = mTextures[index];
		texture->Destroy();
		mPool->Deallocate(texture, sizeof(Texture));
		mTextures.erase(mTextures.begin() + index);

		return eResult::CAVE_OK;
	}

	void GenericRenderer::SetSpriteTexture(uint32_t index, uint32_t textureIndex)
	{
		assert(index < mSprites.size());

		mSprites[index]->SetTextureIndex(textureIndex);
	}

	uint32_t GenericRenderer::GetSpriteTextureIndex(uint32_t index) const
	{
		assert(index < mSprites.size());

		return mSprites[index]->GetTextureIndex();
	}

	void GenericRenderer::SetSpriteSize(uint32_t index, uint32_t width, uint32_t height)
	{
		assert(index < mSprites.size());

		mSprites[index]->SetSize(width, height);
	}

	void GenericRenderer::GetSpriteSize(uint32_t index, uint32_t& outWidth, uint32_t& outHeight) const
	{
		assert(index < mSprites.size());

		mSprites[index]->GetSize(outWidth, outHeight);
	}

	void GenericRenderer::GetSpritePosition(uint32_t index, float& outX, float& outY)
	{
		assert(index < mSprites.size());

		outX = mSprites[index]->GetPositionX();
		outY = mSprites[index]->GetPositionY();
	}

	Float2 GenericRenderer::GetSpritePosition(uint32_t index) const
	{
		assert(index < mSprites.size());

		Float2 position = mSprites[index]->GetPosition();
		return position;
	}
}