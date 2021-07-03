/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"
#include "Device/DeviceResources.h"
#include "Renderer/Camera.h"
#include "Sprite/Sprite.h"
#include "Shader/Shader.h"

namespace cave
{
	class GenericRenderer
	{
	public:
		GenericRenderer();
		virtual ~GenericRenderer();

		GenericRenderer(const GenericRenderer&) = delete;
		GenericRenderer& operator=(const GenericRenderer&) = delete;
		GenericRenderer& operator=(const GenericRenderer&&) = delete;

		virtual eResult Init(Window* window) = 0;
		virtual eResult CreateDeviceDependentResources() = 0;
		virtual eResult CreateWindowSizeDependentResources(Window* window = nullptr) = 0;
    	virtual void Update() = 0;
    	virtual void Render() = 0;
		virtual void Destroy();

		virtual eResult AddSprite(Sprite&& object);
		virtual eResult AddTexture(Texture&& texture);
		virtual eResult RemoveSprite(uint32_t index);
		virtual eResult RemoveTexture(uint32_t index);
		virtual void SetSpriteTexture(uint32_t index, uint32_t textureIndex);
		virtual uint32_t GetSpriteTextureIndex(uint32_t index) const;
		virtual void SetSpriteSize(uint32_t index, uint32_t width, uint32_t height);
		virtual void GetSpriteSize(uint32_t index, uint32_t& outWidth, uint32_t& outHeight) const;
		virtual void GetSpritePosition(uint32_t index, float& outX, float& outY);
		virtual Float2 GetSpritePosition(uint32_t index) const;

		virtual bool WindowShouldClose() = 0;
		DeviceResources* GetDeviceResources() const;
	protected:

		static constexpr size_t RENDERER_MEMORY_SIZE = 1024ul * 1024ul * 10ul;
		MemoryPool* mPool = nullptr;

		Camera* mCamera = nullptr;

#ifdef __WIN32__
		DirectX::XMMATRIX mProjection;
#else
		glm::mat4 mProjection = glm::mat4(1.0f);
#endif
		DeviceResources* mDeviceResources = nullptr;
		uint32_t mIndexCount = 0u;
		uint32_t mFrameCount = 0u;

		std::vector<Sprite*> mSprites;
		std::vector<Texture*> mTextures;
		Shader* mShader;
	};
}