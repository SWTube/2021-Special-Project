/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#ifdef __UNIX__
#include "Sprite/UnixSprite.h"

#include "lodepng.h"

namespace cave
{
	UnixSprite::UnixSprite(Texture* texture, MemoryPool& pool)
		: GenericSprite(texture, pool)
	{
	}
	
	UnixSprite::UnixSprite(const Texture& texture, MemoryPool& pool)
		: GenericSprite(texture, pool)
	{
	}
	
	UnixSprite::UnixSprite(Texture&& texture, MemoryPool& pool)
		: GenericSprite(std::move(texture), pool)
	{
	}

	UnixSprite::UnixSprite(UnixSprite&& other)
		: GenericSprite(std::move(other))
	{
	}

	UnixSprite& UnixSprite::operator=(UnixSprite&& other)
	{
		if (this != &other)
		{
			GenericSprite::operator=(other);
			mVertexArrayObject = other.mVertexArrayObject;
			memcpy(mBuffers, other.mBuffers, BUFFER_COUNT);
			mProgram = other.mProgram;
		}

		return *this;
	}

	UnixSprite::~UnixSprite()
	{
		Destroy();
	}

	eResult UnixSprite::initializeBuffers(uint32_t program)
	{
		srand(time(0));
		eResult result = eResult::CAVE_OK;
		uint32_t glError = GL_NO_ERROR;
		mProgram = program;

		// 14. Create Buffer ---------------------------------------------------------------------------------------------
		glCreateBuffers(Sprite::BUFFER_COUNT, mBuffers);
		if (glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glCreateBuffers error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}
		for (uint32_t i = 0; i < Sprite::BUFFER_COUNT; ++i)
		{
			LOGDF(eLogChannel::GRAPHICS, "buffer: %u", mBuffers[i]);
		}

		// 15. Bind Buffer ---------------------------------------------------------------------------------------------
		// glBindBuffer(GL_ARRAY_BUFFER, mBuffers[Sprite::ARRAY_BUFFER]);
		// if (glError = glGetError(); glError != GL_NO_ERROR)
		// {
		// 	LOGEF(eLogChannel::GRAPHICS, "glBindBuffer error code: 0x%x", glError);
		// 	return eResult::CAVE_FAIL;
		// }
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[Sprite::ELEMENT_ARRAY_BUFFER]);
		// if (glError = glGetError(); glError != GL_NO_ERROR)
		// {
		// 	LOGEF(eLogChannel::GRAPHICS, "glBindBuffer error code: 0x%x", glError);
		// 	return eResult::CAVE_FAIL;
		// }

		// Preparing to Send Data to OpenGL
		// All data must be stored in buffer objects (chunks of memory managed by OpenGL)
		// Common way is to specify the data at the same time as you specify the buffer's size
		glNamedBufferStorage(mBuffers[Sprite::ARRAY_BUFFER], sizeof(mVertices), mVertices, GL_DYNAMIC_STORAGE_BIT);
		if (glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glNamedBufferStorage error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glNamedBufferStorage(mBuffers[Sprite::ELEMENT_ARRAY_BUFFER], sizeof(INDICES), INDICES, GL_DYNAMIC_STORAGE_BIT);
		if (glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glNamedBufferStorage error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glCreateVertexArrays(1u, &mVertexArrayObject);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glCreateVertexArrays error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		return result;
	}

	void UnixSprite::Destroy()
	{
		GenericSprite::Destroy();
		
		glDeleteBuffers(BUFFER_COUNT, mBuffers);
	}

	const uint32_t* UnixSprite::GetBuffers() const
	{
		return mBuffers;
	}

	void UnixSprite::Update()
	{
		// mPosition.X += static_cast<float>(rand() % 10) - 5.0f;
		// mPosition.Y += static_cast<float>(rand() % 10) - 5.0f;
	}

	void UnixSprite::Render()
	{
		int32_t glError = GL_NO_ERROR;

		glBindVertexArray(mVertexArrayObject);
		if (glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glBindVertexArray error code: 0x%x", glError);
		}

		if (!mbNeedsUpdate && (mPosition == mPreviousPosition))
		{
			return;
		}

		mPreviousPosition = mPosition;

		glm::mat4 worldMatrix = glm::mat4(1.0f);
		worldMatrix = glm::translate(worldMatrix, glm::vec3(mPosition.X, mPosition.Y, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		// worldMatrix = glm::translate(worldMatrix, glm::vec3(0.5f * mWidth, 0.5f * mHeight, 0.0f)); // move origin of rotation to center of quad
		// worldMatrix = glm::rotate(worldMatrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		// worldMatrix = glm::translate(worldMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

		worldMatrix = glm::scale(worldMatrix, glm::vec3(mWidth, mHeight, 1.0f)); // last scale

		// LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f %6f", mVertices[0].Position.X, mVertices[0].Position.Y, mVertices[0].Position.Z, mVertices[0].TexCoord.X, mVertices[0].TexCoord.Y);
		// LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f %6f", mVertices[1].Position.X, mVertices[1].Position.Y, mVertices[1].Position.Z, mVertices[1].TexCoord.X, mVertices[1].TexCoord.Y);
		// LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f %6f", mVertices[2].Position.X, mVertices[2].Position.Y, mVertices[2].Position.Z, mVertices[2].TexCoord.X, mVertices[2].TexCoord.Y);
		// LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f %6f", mVertices[3].Position.X, mVertices[3].Position.Y, mVertices[3].Position.Z, mVertices[3].TexCoord.X, mVertices[3].TexCoord.Y);

		LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f", worldMatrix[0][0], worldMatrix[0][1], worldMatrix[0][2], worldMatrix[0][3]);
		LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f", worldMatrix[1][0], worldMatrix[1][1], worldMatrix[1][2], worldMatrix[1][3]);
		LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f", worldMatrix[2][0], worldMatrix[2][1], worldMatrix[2][2], worldMatrix[2][3]);
		LOGIF(eLogChannel::GRAPHICS, "%6f %6f %6f %6f", worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2], worldMatrix[3][3]);


		mbNeedsUpdate = false;

		// mVertices[0]

		mTextureIndex = mTexture->GetIndex();

		int32_t worldLocation = glGetUniformLocation(mProgram, "World");
		if (worldLocation < 0)
		{
			LOGEF(eLogChannel::GRAPHICS, "Failed to get uniform location: %s, location: %d, error code: 0x%x", "World", worldLocation, glGetError());
		}
		// glUniformMatrix4fv(worldLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix4fv(worldLocation, 1, false, glm::value_ptr(worldMatrix));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glUniformMatrix4fv location: %u, error code: 0x%x", worldLocation, glError);
		}
	}
} // namespace cave
#endif