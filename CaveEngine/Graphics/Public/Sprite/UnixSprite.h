/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreMinimal.h"
#include "Sprite/GenericSprite.h"

#ifdef __UNIX__
namespace cave
{
	class UnixSprite final : public GenericSprite
	{
	public:
		UnixSprite() = delete;
		UnixSprite(Texture* texture, MemoryPool& pool);
		UnixSprite(const Texture& texture, MemoryPool& pool);
		UnixSprite(Texture&& texture, MemoryPool& pool);
		UnixSprite(const UnixSprite& object) = default;
		UnixSprite(UnixSprite&& object);
		UnixSprite& operator=(const UnixSprite& object) = default;
		UnixSprite& operator=(UnixSprite&& object);
		virtual ~UnixSprite();

		void Destroy() override;
		void Update() override;
		void Render() override;

		const uint32_t* GetBuffers() const;
		constexpr uint32_t GetVertexArrayObject() const;

		static constexpr uint32_t ARRAY_BUFFER = 0u;
		static constexpr uint32_t ELEMENT_ARRAY_BUFFER = 1u;
		static constexpr uint32_t BUFFER_COUNT = 2u;
	private:
		eResult initializeBuffers(uint32_t program) override;
		uint32_t mVertexArrayObject = 0u;
		uint32_t mBuffers[BUFFER_COUNT] = { 0u, };
		uint32_t mProgram = 0u;
	};

	constexpr uint32_t UnixSprite::GetVertexArrayObject() const
	{
		return mVertexArrayObject;
	}

	typedef UnixSprite Sprite;
} // namespace cave
#endif