/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Sprite/WindowsSprite.h"


#ifdef __WIN32__
import DdsTextureLoader;

namespace cave
{
	WindowsSprite::WindowsSprite(Texture* texture, MemoryPool& pool)
		: GenericSprite(texture, pool)
	{
	}
	
	WindowsSprite::WindowsSprite(const Texture& texture, MemoryPool& pool)
		: GenericSprite(texture, pool)
	{
	}

	WindowsSprite::WindowsSprite(Texture&& texture, MemoryPool& pool)
		: GenericSprite(std::move(texture), pool)
	{
	}

	WindowsSprite::WindowsSprite(const WindowsSprite& other)
		: GenericSprite(other)
		, mDevice(other.mDevice)
		, mContext(other.mContext)
		, mVertexBuffer(other.mVertexBuffer)
		, mIndexBuffer(other.mIndexBuffer)
		, mVertexLayout(other.mVertexLayout)
		, mTextureRv(other.mTextureRv)
	{
	}

	WindowsSprite::WindowsSprite(WindowsSprite&& other)
		: GenericSprite(std::move(other))
	{
		mDevice = other.mDevice;
		mContext = other.mContext;
		mVertexBuffer = other.mVertexBuffer;
		mIndexBuffer = other.mIndexBuffer;
		mVertexLayout = other.mVertexLayout;
		mTextureRv = other.mTextureRv;

		other.Destroy();
	}

	WindowsSprite& WindowsSprite::operator=(const WindowsSprite& other)
	{
		if (this != &other)
		{
			GenericSprite::operator=(other);
			mDevice = other.mDevice;
			mContext = other.mContext;
			mVertexBuffer = other.mVertexBuffer;
			mIndexBuffer = other.mIndexBuffer;
			mVertexLayout = other.mVertexLayout;
			mTextureRv = other.mTextureRv;
		}

		return *this;
	}

	WindowsSprite& WindowsSprite::operator=(WindowsSprite&& other)
	{
		if (this != &other)
		{
			GenericSprite::operator=(std::move(other));
			mDevice = other.mDevice;
			mContext = other.mContext;
			mVertexBuffer = other.mVertexBuffer;
			mIndexBuffer = other.mIndexBuffer;
			mVertexLayout = other.mVertexLayout;
			mTextureRv = other.mTextureRv;

			other.Destroy();
		}

		return *this;
	}

	WindowsSprite::~WindowsSprite()
	{
		Destroy();
	}

	eResult WindowsSprite::initializeBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		assert(device != nullptr && context != nullptr);
		mDevice = device;
		mContext = context;
		int32_t result = S_OK;

		// create static vertex buffer
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexT) * VERTICES_COUNT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = mVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		result = device->CreateBuffer(&bufferDesc, &vertexData, &mVertexBuffer);
		if(FAILED(result))
		{
			return eResult::CAVE_FAIL;
		}

		// create static index buffer
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(uint8_t) * sizeof(INDICES);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		vertexData.pSysMem = INDICES;
		result = device->CreateBuffer(&bufferDesc, &vertexData, &mIndexBuffer);
		if(FAILED(result))
		{
			return eResult::CAVE_FAIL;
		}

		mWorld = DirectX::XMMatrixIdentity();

		return eResult::CAVE_OK;
	}

	void WindowsSprite::Destroy()
	{
		GenericSprite::Destroy();
		
		if (mTextureRv != nullptr)
		{
			mTextureRv->Release();
			mTextureRv = nullptr;
		}
		if (mVertexBuffer != nullptr)
		{
			mVertexBuffer->Release();
			mVertexBuffer = nullptr;
		}
		if (mIndexBuffer != nullptr)
		{
			mIndexBuffer->Release();
			mIndexBuffer = nullptr;
		}
		if (mVertexLayout != nullptr)
		{
			mVertexLayout->Release();
			mVertexLayout = nullptr;
		}
		if (mContext != nullptr)
		{
			mContext->Release();
			mContext = nullptr;
		}
		if (mDevice != nullptr)
		{
			mDevice->Release();
			mDevice = nullptr;
		}
	}

	void WindowsSprite::Update()
	{
	}

	void WindowsSprite::Render()
	{
		//
		// Update variables for the first cube
		//
		float left = 0.0f;
		float right = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;
		Vertex* vertices = nullptr;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Vertex* verticesPtr = nullptr;
		eResult result = eResult::CAVE_OK;

		if (!mbNeedsUpdate && (mPosition == mPreviousPosition))
		{
			return;
		}

		mPreviousPosition = mPosition;

		left = static_cast<float>((mScreenWidth / 2) * -1) + mPosition.X - static_cast<float>(mWidth) / 2.0f;
		right = left + static_cast<float>(mWidth);
		top = static_cast<float>((mScreenHeight / 2) * -1) - mPosition.Y + static_cast<float>(mHeight) / 2.0f;
		bottom = top - static_cast<float>(mHeight);

		mVertices[0] = std::move(VertexT(Float3( left,    top, 0.0f), Float2(0.0f, 0.0f)));		// top left
		mVertices[1] = std::move(VertexT(Float3( right,    top, 0.0f), Float2(1.0f, 0.0f)));	// top right
		mVertices[2] = std::move(VertexT(Float3(right, bottom, 0.0f), Float2(1.0f, 1.0f)));		// bottom right
		mVertices[3] = std::move(VertexT(Float3( left, bottom, 0.0f), Float2(0.0f, 1.0f)));		// bottom left

		HRESULT hResult = mContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		verticesPtr = reinterpret_cast<VertexT*>(mappedResource.pData);
		memcpy(verticesPtr, reinterpret_cast<void*>(mVertices), (sizeof(VertexT) * VERTICES_COUNT));
		mContext->Unmap(mVertexBuffer, 0);

		// ���� context�� �����ֱ�
		// Set vertex buffer
		uint32_t stride = sizeof(VertexT);
		uint32_t offset = 0;
		mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

		// Set index buffer
		mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// Set primitive topology
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
} // namespace cave

#endif