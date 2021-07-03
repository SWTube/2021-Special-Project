/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "lodepng.h"

#include "Debug/Log.h"
#include "Texture/Texture.h"

#ifdef __WIN32__
	import DdsTextureLoader;
#endif

namespace cave
{
	int32_t Texture::msTextureCount = -1;

	Texture::Texture(const std::filesystem::path& filePath, eTextureFormat textureFormat, MemoryPool& pool)
		: mPool(&pool)
		, mFormat(textureFormat)
		, mTarget(++msTextureCount)
	{
#ifdef __WIN32__
		mFilePath /= L"CaveEngine/Graphics/Resource";
		std::filesystem::create_directories(mFilePath / L"Textures");
		mFilePath /= L"Textures";
		mFilePath /= filePath;

		mTexture = reinterpret_cast<TexturePointer*>(mPool->Allocate(sizeof(TexturePointer)));
		mTexture->ReferenceCount = 1u;
		mTexture->Texture = nullptr;
#else
		mFilePath /= "CaveEngine/Graphics/Resource";
		std::filesystem::create_directories(mFilePath / "Textures");
		mFilePath /= "Textures";
		mFilePath /= filePath;

		mTexture = reinterpret_cast<TexturePointer*>(mPool->Allocate(sizeof(TexturePointer)));
		mTexture->ReferenceCount = 1u;
		mTexture->Texture = nullptr;
#endif
	}

	Texture::Texture(const Texture& other)
		: mPool(other.mPool)
		, mFilePath(other.mFilePath)
		, mWidth(other.mWidth)
		, mHeight(other.mHeight)
		, mFormat(other.mFormat)
		, mTexture(other.mTexture)
		, mIndex(other.mIndex)
		, mTarget(other.mTarget)
	{
		if (mTexture != nullptr)
		{
			++mTexture->ReferenceCount;
		}
	}

	Texture::Texture(Texture&& other)
		: mPool(other.mPool)
		, mFilePath(other.mFilePath)
		, mWidth(other.mWidth)
		, mHeight(other.mHeight)
		, mFormat(other.mFormat)
		, mTexture(other.mTexture)
		, mIndex(other.mIndex)
		, mTarget(other.mTarget)
	{
		other.mPool = nullptr;
		other.mFilePath.clear();
		other.mTexture = nullptr;
		other.mIndex = -1;
		other.mTarget = 0u;
	}

	Texture& Texture::operator=(const Texture& other)
	{
		if (this != &other)
		{
			mPool = other.mPool;
			mFilePath = other.mFilePath;
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			mFormat = other.mFormat;
			if (mTexture != nullptr)
			{
				--mTexture->ReferenceCount;
			}
			mTexture = other.mTexture;
			if (mTexture != nullptr)
			{
				++mTexture->ReferenceCount;
			}
			mIndex = other.mIndex;
		}

		return *this;
	}

	Texture& Texture::operator=(Texture&& other)
	{
		if (this != &other)
		{
			mPool = other.mPool;
			mFilePath = other.mFilePath;
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			mFormat = other.mFormat;
			if (mTexture != nullptr)
			{
				--mTexture->ReferenceCount;
			}
			mTexture = other.mTexture;
			mIndex = other.mIndex;
			mTarget = other.mTarget;

			other.mPool = nullptr;
			other.mFilePath.clear();
			other.mTexture = nullptr;
			other.mIndex = 0u;
			other.mTarget = -1;
		}

		return *this;
	}

	Texture::~Texture()
	{
		Destroy();
	}

	void Texture::Init()
	{
#ifdef __WIN32__
		const wchar_t* extension = mFilePath.extension().c_str();
		if (wcsncmp(extension, L".png", 4) == 0)
		{
			uint32_t error = 0u;
			unsigned char* pngData = nullptr;
			switch (mFormat)
			{
			case eTextureFormat::RGB:
				error = lodepng_decode24_file(&pngData, &mWidth, &mHeight, mFilePath.string().c_str());
				break;
			case eTextureFormat::RGBA:
				error = lodepng_decode32_file(&pngData, &mWidth, &mHeight, mFilePath.string().c_str());
				break;
			default:
				assert(false);
				break;
			}
			//LoadFromPngData(device,pngImage);
			if (error != 0)
			{
				LOGEF(eLogChannel::GRAPHICS, "The png file %s cannot be loaded. Error Code: %u", mFilePath.string().c_str(), error);
			}
		}
		else if (wcsncmp(extension, L".dds", 4) == 0)
		{

		}
#else
		// 16. Load Textures ---------------------------------------------------------------------------------------------

		// LOGDF(eLogChannel::GRAPHICS, std::cout, "vector size: %lu", TextureData.size());
		glCreateTextures(GL_TEXTURE_2D, 1u, &mIndex);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glCreateTextures error code: 0x%x", glError);
		}

		glTextureParameteri(mIndex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(mIndex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(mIndex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(mIndex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		const char* extension = mFilePath.extension().c_str();
		if (strncmp(extension, ".png", 4) == 0)
		{
			uint32_t error = 0u;
			switch (mFormat)
			{
			case eTextureFormat::RGB:
				{
					error = lodepng_decode24_file(&mTexture->Texture, &mWidth, &mHeight, mFilePath.c_str());

					glTextureStorage2D(mIndex, 1, GL_RGB8, mWidth, mHeight);
					if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
					{
						LOGEF(eLogChannel::GRAPHICS, std::cerr, "glTextureStorage2D error code: 0x%x", glError);
					}
					glTextureSubImage2D(mIndex, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mTexture->Texture);
					if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
					{
						LOGEF(eLogChannel::GRAPHICS, std::cerr, "glTextureSubImage2D error code: 0x%x", glError);
					}
					break;
				}
			case eTextureFormat::RGBA:
				{
					error = lodepng_decode32_file(&mTexture->Texture, &mWidth, &mHeight, mFilePath.c_str());

					glTextureStorage2D(mIndex, 1, GL_RGBA8, mWidth, mHeight);
					if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
					{
						LOGEF(eLogChannel::GRAPHICS, std::cerr, "glTextureStorage2D error code: 0x%x", glError);
					}
					glTextureSubImage2D(mIndex, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, mTexture->Texture);
					if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
					{
						LOGEF(eLogChannel::GRAPHICS, std::cerr, "glTextureSubImage2D error code: 0x%x", glError);
					}
					break;
				}
			default:
				assert(false);
				break;
			}
			
			if (error != 0)
			{
				LOGEF(eLogChannel::GRAPHICS, std::cerr, "The png file %s cannot be loaded. Error Code: %u", mFilePath.c_str(), error);
			}

			// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
			glGenerateTextureMipmap(mIndex);
			if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
			{
				LOGEF(eLogChannel::GRAPHICS, std::cerr, "glGenerateTextureMipmap error code: 0x%x", glError);
			}
			// glCreateSamplers(1, &msBackgroundSampler);
			// LOGDF(eLogChannel::GRAPHICS, std::cout, "sampler: %u", msBackgroundSampler);
			// glBindSampler(msBackgroundIndex, msBackgroundSampler);
			
			if (mTexture != nullptr)
			{
				mTexture->Destroy();
				mTexture = nullptr;
			}
		}
		else if (strncmp(extension, ".dds", 4) == 0)
		{

		}
#endif
	}

	void Texture::DeleteTexture()
	{
		glDeleteTextures(1u, &mIndex);
	}

	void Texture::Destroy()
	{
#if !defined(__WIN32__)
		glDeleteTextures(1u, &mIndex);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(cave::eLogChannel::GRAPHICS, std::cerr, "glDeleteTextures error code: 0x%x", glError);
		}
#endif
		if (mTexture != nullptr)
		{
			if (--mTexture->ReferenceCount == 0)
			{
				--msTextureCount;
			}
			mTexture->~TexturePointer();
			
			mPool->Deallocate(mTexture, sizeof(TexturePointer));
			mTexture = nullptr;
		}
	}

	MemoryPool* Texture::GetMemoryPool()
	{
		return mPool;
	}

	const char* Texture::GetCStringFilePath() const
	{
#ifdef __WIN32__
		return mFilePath.string().c_str();
#else
		return mFilePath.c_str();
#endif
	}

#ifdef __WIN32__
	void Texture::LoadFromPngData(ID3D11Device* device, unsigned char* pngData)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pngData;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;

		ID3D11Texture2D* pTexture2D = nullptr;
		auto result = device->CreateTexture2D(&desc, &subResource, &pTexture2D);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		result = device->CreateShaderResourceView(pTexture2D, &srvDesc, &mTexture->Texture);
	}
#endif
} // namespace cave
