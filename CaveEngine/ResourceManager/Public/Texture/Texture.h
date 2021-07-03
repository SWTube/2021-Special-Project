/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include <filesystem>

#include "ResourceManagerApiPch.h"

#include "CoreGlobals.h"
#include "CoreTypes.h"

namespace cave
{
	enum class eTextureFormat
	{
		RGB,
		RGBA,
	};

	struct Texture final
	{
	public:
		Texture(const std::filesystem::path& filePath, eTextureFormat textureFormat = eTextureFormat::RGBA, MemoryPool& pool = gCoreMemoryPool);
		Texture(const Texture& other);
		Texture(Texture&& other);
		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other);
		~Texture();
		MemoryPool* GetMemoryPool();

		void Init();
		void Destroy();
		void DeleteTexture();

#ifdef __WIN32__
		constexpr ID3D11ShaderResourceView* GetTexture();
#else
		constexpr uint8_t* GetTexture();
#endif
		const char* GetCStringFilePath() const;
		constexpr const std::filesystem::path& GetFilePath() const;
		constexpr uint32_t GetWidth() const;
		constexpr uint32_t GetHeight() const;
		constexpr eTextureFormat GetFormat() const;
		constexpr uint32_t GetIndex() const;
		constexpr int32_t GetTarget() const;
	private:

#ifdef __WIN32__
		void LoadFromPngData(ID3D11Device* device ,unsigned char* pngData);
#endif
		typedef struct TexturePointer
		{
			uint32_t ReferenceCount = 0u;
#ifdef __WIN32__
			ID3D11ShaderResourceView* Texture = nullptr;
#else
			uint8_t* Texture = nullptr;
#endif
			constexpr TexturePointer()
				: ReferenceCount(0u)
				, Texture(nullptr)
			{
			}

			constexpr TexturePointer(const TexturePointer& other) = delete;

			constexpr TexturePointer(TexturePointer&& other) noexcept
				: ReferenceCount(other.ReferenceCount)
				, Texture(other.Texture)
			{
#ifdef __WIN32__
				other.Texture->Release();
#endif
				other.Texture = nullptr;
				other.ReferenceCount = 0u;
			}

			constexpr TexturePointer& operator=(const TexturePointer& other) = delete;

			~TexturePointer()
			{
				Destroy();
			}

			constexpr void Destroy()
			{
				if (Texture != nullptr && ReferenceCount == 0u)
				{
#ifdef __WIN32__
					Texture->Release();
#endif
#ifdef __UNIX__
					Memory::Free(Texture);
#endif
					Texture = nullptr;
				}
			}
		} TexturePointer;

		static int32_t msTextureCount;
		
		MemoryPool* mPool = nullptr;
		std::filesystem::path mFilePath = PROJECT_DIR;
		uint32_t mWidth = 0u;
		uint32_t mHeight = 0u;
		eTextureFormat mFormat = eTextureFormat::RGBA;
		TexturePointer* mTexture = nullptr;
		uint32_t mIndex = 0u;
		int32_t mTarget = -1;
	};

#ifdef __WIN32__
	constexpr ID3D11ShaderResourceView* Texture::GetTexture()
#else
	constexpr uint8_t* Texture::GetTexture()
#endif
	{
		if (mTexture != nullptr)
		{
			return mTexture->Texture;
		}

		return nullptr;
	}

	constexpr uint32_t Texture::GetIndex() const
	{
		return mIndex;
	}

	constexpr const std::filesystem::path& Texture::GetFilePath() const
	{
		return mFilePath;
	}

	constexpr uint32_t Texture::GetWidth() const
	{
		return mWidth;
	}

	constexpr uint32_t Texture::GetHeight() const
	{
		return mHeight;
	}

	constexpr eTextureFormat Texture::GetFormat() const
	{
		return mFormat;
	}

	constexpr int32_t Texture::GetTarget() const
	{
		return mTarget;
	}
} // namespace cave
