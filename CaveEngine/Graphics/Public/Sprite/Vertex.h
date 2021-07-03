/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"

namespace cave
{
	//--------------------------------------------------------------------------------------
	// Structures
	//--------------------------------------------------------------------------------------
	typedef struct Float3
	{
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;

		Float3() = default;
		Float3(const Float3& other) = default;
		Float3& operator=(const Float3& other) = default;
		constexpr Float3(const Float3&& other);
		constexpr Float3& operator=(const Float3&& other);
		constexpr Float3(float x, float y, float z);
		constexpr Float3(float* array);

		constexpr bool operator==(const Float3& rhs) const;
	} Float3;

	constexpr Float3::Float3(const Float3&& other)
	{
		if (this != &other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
	}

	constexpr Float3& Float3::operator=(const Float3&& other)
	{
		if (this != &other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}

		return *this;
	}

	constexpr Float3::Float3(float* array)
		: X(array[0])
		, Y(array[1])
		, Z(array[2])
	{
	}

	constexpr Float3::Float3(float x, float y, float z)
		: X(x)
		, Y(y)
		, Z(z)
	{
	}

	constexpr bool Float3::operator==(const Float3& rhs) const
	{
		return (X == rhs.X) && (Y == rhs.Y) && (Z == rhs.Z);
	}

	struct Float2
	{
		float X = 0.0f;
		float Y = 0.0f;
		Float2() = default;
		Float2(const Float2& other) = default;
		Float2& operator=(const Float2& other) = default;
		constexpr Float2(const Float2&& other);
		constexpr Float2& operator=(const Float2&& other);
		constexpr Float2(float x, float y);
		constexpr Float2(float* array);
	};

	constexpr Float2::Float2(const Float2&& other)
		: X(other.X)
		, Y(other.Y)
	{
	}

	constexpr Float2& Float2::operator=(const Float2&& other)
	{
		X = other.X;
		Y = other.Y;

		return *this;
	}

	constexpr Float2::Float2(float* array)
		: X(array[0])
		, Y(array[1])
	{
	}

	constexpr Float2::Float2(float x, float y)
		: X(x)
		, Y(y)
	{
	}

	constexpr uint32_t VERTEX_POSITION = 0b01;
	constexpr uint32_t VERTEX_TEXCOORD = 0b10;


	typedef struct Vertex
	{
	public:
		Float3 Position = Float3(0.0f, 0.0f, 0.0f);
		uint32_t VertexFlag = VERTEX_POSITION;

		Vertex() = default;
		Vertex(const Vertex& other) = default;
		Vertex& operator=(const Vertex& other) = default;
		constexpr Vertex(const Vertex&& other);
		constexpr Vertex& operator=(const Vertex&& other);
		constexpr Vertex(float x, float y, float z);
		constexpr Vertex(float* array);
		constexpr Vertex(const Float3& array);
		constexpr Vertex(const Float3&& array);
		constexpr uint32_t GetSize() const;

		constexpr bool operator==(const Vertex& rhs) const;
	} Vertex;

	constexpr Vertex& Vertex::operator=(const Vertex&& other)
	{
		if (this != &other)
		{
			Position = other.Position;
		}

		return *this;
	}

	constexpr Vertex::Vertex(const Vertex&& other)
	{
		if (this != &other)
		{
			Position = other.Position;
		}
	}

	constexpr Vertex::Vertex(float* array)
		: Position(Float3(array))
	{
	}

	constexpr Vertex::Vertex(const Float3& array)
		: Position(array)
	{
	}

	constexpr Vertex::Vertex(const Float3&& array)
		: Position(array)
	{
	}

	constexpr Vertex::Vertex(float x, float y, float z)
		: Position(Float3(x, y, z))
	{
	}

	constexpr uint32_t Vertex::GetSize() const
	{
		uint32_t size = 0u;
		if (VertexFlag & VERTEX_POSITION)
		{
			size += 3u;
		}

		if (VertexFlag & VERTEX_TEXCOORD)
		{
			size += 2u;
		}

		size = size * sizeof(float) + sizeof(VertexFlag);

		if (size | 0b10 || size | 0b01)
		{
			size &= ~(0b11);
			size += 0b100;
		}

		return size;
	}

	constexpr bool Vertex::operator==(const Vertex& rhs) const
	{
		return (Position == rhs.Position) && (VertexFlag == rhs.VertexFlag);
	}

	typedef struct VertexT : public Vertex
	{
	public:
		Float2 TexCoord = Float2(0.0f, 0.0f);

		VertexT() = default;
		VertexT(const VertexT& other) = default;
		VertexT& operator=(const VertexT& other) = default;
		constexpr VertexT(const VertexT&& other);
		constexpr VertexT& operator=(const VertexT&& other);
		constexpr VertexT(float posX, float posY, float posZ, float texX, float texY);
		constexpr VertexT(float* array);
		constexpr VertexT(const Float3& position, const Float2& texCoord);
		constexpr VertexT(const Float3&& position, const Float2&& texCoord);
	} VertexT;

	constexpr VertexT::VertexT(const VertexT&& other)
	{
		if (this != &other)
		{
			Position = other.Position;
			TexCoord = other.TexCoord;
			VertexFlag |= VERTEX_TEXCOORD;
		}
	}

	constexpr VertexT& VertexT::operator=(const VertexT&& other)
	{
		if (this != &other)
		{
			Position = other.Position;
			TexCoord = other.TexCoord;
			VertexFlag |= VERTEX_TEXCOORD;
		}

		return *this;
	}

	constexpr VertexT::VertexT(float* array)
		: Vertex(array)
		, TexCoord(Float2(array[3], array[4]))
	{
		VertexFlag |= VERTEX_TEXCOORD;
	}

	constexpr VertexT::VertexT(const Float3& position, const Float2& texCoord)
		: Vertex(position)
		, TexCoord(texCoord)
	{
		VertexFlag |= VERTEX_TEXCOORD;
	}

	constexpr VertexT::VertexT(const Float3&& position, const Float2&& texCoord)
		: Vertex(std::move(position))
		, TexCoord(std::move(texCoord))
	{
		VertexFlag |= VERTEX_TEXCOORD;
	}

	constexpr VertexT::VertexT(float posX, float posY, float posZ, float texX, float texY)
		: Vertex(posX, posY, posZ)
		, TexCoord(Float2(texX, texY))
	{
		VertexFlag |= VERTEX_TEXCOORD;
	}
} // namespace cave
