/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#include "Containers/Array.h"

namespace cave
{
	constexpr FFloatArray::FFloatArray(size_t size)
		: mSize(size)
		, mData(new float[size])
	{ }
	constexpr FFloatArray::FFloatArray(const FFloatArray& other)
		: mSize(other.mSize)
		, mData(new float[other.mSize])
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			mData[i] = other.mData[i];
		}
	}
	constexpr FFloatArray::FFloatArray(FFloatArray&& other) noexcept
		: mSize(other.mSize)
		, mData(other.mData)
	{
		other.mData = nullptr;
	}

	FFloatArray::~FFloatArray()
	{
		if (mData != nullptr)
		{
			delete[] mData;
		}
	}

	constexpr size_t FFloatArray::GetArrayLength() const
	{
		return mSize;
	}

	constexpr float& FFloatArray::operator[](size_t index)
	{
		return mData[index];
	}

	constexpr float& FFloatArray::operator*()
	{
		return *mData;
	}

	constexpr float* FFloatArray::operator+(size_t index)
	{
		return mData + index;
	}
}
