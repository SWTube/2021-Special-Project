/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreTypes.h"
// #include "Renderer/GenericRenderer.h"
#include "Sprite/Vertex.h"

// #include GET_PLATFORM_HEADER(Camera.h)

namespace cave
{
	class Camera final
	{
	public:
		constexpr Camera() = default;
		constexpr Camera(const Camera& other) = default;
		constexpr Camera(Camera&& other) = default;
		constexpr Camera& operator=(const Camera& other) = default;
		constexpr Camera& operator=(Camera&& other) = default;
		virtual ~Camera() = default;

		constexpr void SetPosition(float x, float y, float z);
		constexpr void SetPosition(const Float3& position);
		constexpr void SetPosition(Float3&& position);

		constexpr void SetRotation(float x, float y, float z);
		constexpr void SetRotation(const Float3& rotation);
		constexpr void SetRotation(Float3&& rotation);

		constexpr const Float3& GetPosition() const;
		constexpr const Float3& GetRotation() const;

		void Render();
#ifdef __WIN32__
		constexpr const DirectX::XMMATRIX& GetViewMatrix() const;
#else
		constexpr const glm::mat4& GetViewMatrix() const;
#endif
	private:
		Float3 mPosition = Float3(0.0f, 0.0f, 0.0f);
		Float3 mRotation = Float3(0.0f, 0.0f, 0.0f);
#ifdef __WIN32__
		DirectX::XMMATRIX mView = DirectX::XMMatrixIdentity();
#else
		glm::mat4 mView  = glm::mat4(1.0f);
#endif
	};

	constexpr void Camera::SetPosition(float x, float y, float z)
	{
		mPosition.X = x;
		mPosition.Y = y;
		mPosition.Z = z;
	}

	constexpr void Camera::SetPosition(const Float3& position)
	{
		mPosition = position;
	}

	constexpr void Camera::SetPosition(Float3&& position)
	{
		mPosition = position;
	}

	constexpr void Camera::SetRotation(float x, float y, float z)
	{
		mRotation.X = x;
		mRotation.Y = y;
		mRotation.Z = z;
	}

	constexpr void Camera::SetRotation(const Float3& rotation)
	{
		mRotation = rotation;
	}

	constexpr void Camera::SetRotation(Float3&& rotation)
	{
		mRotation = rotation;
	}

	constexpr const Float3& Camera::GetPosition() const
	{
		return mPosition;
	}

	constexpr const Float3& Camera::GetRotation() const
	{
		return mRotation;
	}

#ifdef __WIN32__
	constexpr const DirectX::XMMATRIX& Camera::GetViewMatrix() const
#else
	constexpr const glm::mat4& Camera::GetViewMatrix() const
#endif
	{
		return mView;
	}
} // namespace cave
