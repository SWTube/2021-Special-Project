/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include <filesystem>

#include "Shader/UnixShader.h"
#include "Sprite/Vertex.h"

#ifdef __UNIX__
namespace cave
{
	UnixShader::UnixShader(const std::filesystem::path& vertexShaderFilePath, const std::filesystem::path& fragmentShaderFilePath, MemoryPool& pool)
		: GenericShader(vertexShaderFilePath, fragmentShaderFilePath, pool)
	{
	}

	UnixShader::UnixShader(UnixShader&& other)
		: GenericShader(std::move(other))
		, mProgram(other.mProgram)
	{
	}

	UnixShader& UnixShader::operator=(UnixShader&& other)
	{
		if (this != &other)
		{
			GenericShader::operator=(std::move(other));
			mProgram = other.mProgram;
		}

		return *this;
	}

	void UnixShader::Render(uint32_t indexCount, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, uint32_t texture)
	{
		glUseProgram(mProgram);
		int32_t viewLocation = glGetUniformLocation(mProgram, "View");
		if (viewLocation < 0)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "Failed to get uniform location: %s, location: %d, error code: 0x%x", "View", viewLocation, glGetError());
			assert(false);
		}
		glUniformMatrix4fv(viewLocation, 1, false, glm::value_ptr(viewMatrix));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glUniformMatrix4fv location: %u, error code: 0x%x", viewLocation, glError);
		}

		int32_t projectionLocation = glGetUniformLocation(mProgram, "Projection");
		if (projectionLocation < 0)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "Failed to get uniform location: %s, error code: 0x%x", "Projection", glGetError());
			assert(false);
		}
		glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projectionMatrix));
		// glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glUniformMatrix4fv location: %u, error code: 0x%x", projectionMatrix, glError);
		}

		glBindTextureUnit(0u, texture);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glBindTextureUnit error code: 0x%x, %u", glError, texture);
		}

		// glUseProgram(mProgram);

		// 5. Render ---------------------------------------------------------------------------------------------
		// Sending Data to OpenGL
			// Drawing == transferring vertex data to the OpenGL server
			// vertex == bundle of data values that are processed together
				// almost always includes positional data
				// values needed to determine the pixel's final color
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, nullptr);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glDrawElements error code: 0x%x", glError);
		}
	}
	
	eResult UnixShader::Compile()
	{
		// 11. Compile Shaders ---------------------------------------------------------------------------------------------
		ShaderInfo shaders[] =
		{
			{ .type=GL_VERTEX_SHADER, .filename=mVertexShaderFilePath.c_str(), .shader=0u },
			{ .type=GL_FRAGMENT_SHADER, .filename=mFragmentShaderFilePath.c_str(), .shader=0u },
			{ .type=GL_NONE, .filename=nullptr, .shader=0u }
		};
		
		eResult error = compileShaderFromFile(shaders);
		if (error != eResult::CAVE_OK)
		{
			LOGE(eLogChannel::GRAPHICS, std::cout, "The shader files cannot be compiled.");
			return error;
		}

		return eResult::CAVE_OK;
	}

	eResult UnixShader::SetInputLayout(const Sprite& sprite)
	{
		// 12. Define, create, set the input layout ---------------------------------------------------------------------------------------------
		uint32_t vertexArrayObject = sprite.GetVertexArrayObject();

		glVertexArrayVertexBuffer(vertexArrayObject, 0, sprite.GetBuffers()[Sprite::ARRAY_BUFFER], 0, sizeof(VertexT));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayVertexBuffer error code: 0x%x", glError);
		}
		glVertexArrayElementBuffer(vertexArrayObject, sprite.GetBuffers()[Sprite::ELEMENT_ARRAY_BUFFER]);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayElementBuffer error code: 0x%x", glError);
		}
		
		// glBindVertexArray(mVertexArrayObject);
		// if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		// {
		// 	LOGEF(eLogChannel::GRAPHICS, std::cerr, "glBindVertexArray error code: 0x%x", glError);
		// 	return eResult::CAVE_FAIL;
		// }

		// glVertexAttribPointer(V_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexT), BUFFER_OFFSET(0u));
		// if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		// {
		// 	LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexAttribPointer error code: 0x%x", glError);
		// 	return eResult::CAVE_FAIL;
		// }

		glEnableVertexArrayAttrib(vertexArrayObject, V_POSITION);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glEnableVertexArrayAttrib error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}
		

		// glVertexAttribPointer(V_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(VertexT), BUFFER_OFFSET(sizeof(VertexT)));
		// if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		// {
		// 	LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexAttribPointer error code: 0x%x", glError);
		// 	return eResult::CAVE_FAIL;
		// }
		glEnableVertexArrayAttrib(vertexArrayObject, V_TEX_COORD);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glEnableVertexArrayAttrib error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glVertexArrayAttribFormat(vertexArrayObject, V_POSITION, 3, GL_FLOAT, GL_FALSE, 0u);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayAttribFormat error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glVertexArrayAttribFormat(vertexArrayObject, V_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex));
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayAttribFormat error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glVertexArrayAttribBinding(vertexArrayObject, 0, 0);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayAttribBinding error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		glVertexArrayAttribBinding(vertexArrayObject, 1, 0);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glVertexArrayAttribBinding error code: 0x%x", glError);
			return eResult::CAVE_FAIL;
		}

		return eResult::CAVE_OK;
	}

	uint32_t UnixShader::GetProgram()
	{
		return mProgram;
	}

	//--------------------------------------------------------------------------------------
	// Helper for compiling shaders with D3DCompile
	//
	// With VS 11, we could load up prebuilt .cso files instead...
	//--------------------------------------------------------------------------------------
	eResult UnixShader::compileShaderFromFile(ShaderInfo* shaders)
	{
		mProgram = loadShaders(shaders);
		glUseProgram(mProgram);
		if (uint32_t glError = glGetError(); glError != GL_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, std::cerr, "glUseProgram error code: 0x%x", glError);
		}

		return eResult::CAVE_OK;
	}


	uint32_t UnixShader::loadShaders(ShaderInfo* shaders)
	{
		if (shaders == nullptr)
		{
			return 0;
		}

		GLuint program = glCreateProgram();

		ShaderInfo* entry = shaders;
		while (entry->type != GL_NONE)
		{
			GLuint shader = glCreateShader(entry->type);

			entry->shader = shader;

			const char* source = readShader(entry->filename);
			if (source == nullptr)
			{
				for (entry = shaders; entry->type != GL_NONE; ++entry)
				{
					glDeleteShader( entry->shader );
					entry->shader = 0;
				}

				return 0;
			}

			glShaderSource(shader, 1, &source, nullptr);
			delete [] source;

			glCompileShader(shader);

			int32_t compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled)
			{
#ifdef CAVE_BUILD_DEBUG
			int32_t len;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

			char* log = new char[len+1];
			glGetShaderInfoLog( shader, len, &len, log );
			std::cerr << "Shader compilation failed: " << log << std::endl;
			LOGAF(eLogChannel::GRAPHICS, std::cout, "%s Compilation Failed: %s", entry->filename, log);
			delete [] log;
#endif /* DEBUG */

				return 0;
			}

			glAttachShader(program, shader);
			
			++entry;
		}

		glLinkProgram(program);

		int32_t linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
#ifdef CAVE_BUILD_DEBUG
			int32_t len;
			glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

			char* log = new char[len + 1];
			glGetProgramInfoLog( program, len, &len, log );
			LOGAF(eLogChannel::GRAPHICS, std::cout, "Shader Linking Failed: %s", log);
			delete [] log;
#endif /* DEBUG */

			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			
			return 0;
		}

		return program;
	}

	const char* UnixShader::readShader(const char* filename)
	{
#ifdef __WIN32__
		FILE* infile;
		fopen_s( &infile, filename, "rb" );
#else
		FILE* infile = fopen(filename, "rb");
#endif // __WIN32__

		if (!infile)
		{
#ifdef CAVE_BUILD_DEBUG
			LOGAF(eLogChannel::GRAPHICS, std::cout, "Unable to open file '%s' ", filename);
#endif /* DEBUG */
			return nullptr;
		}

		fseek(infile, 0, SEEK_END);
		int len = ftell(infile);
		fseek(infile, 0, SEEK_SET);

		char* source = new char[len + 1];

		if (fread(source, 1, len, infile) < 1)
		{
			return nullptr;
		}
		fclose(infile);

		source[len] = 0;

		return const_cast<const char*>(source);
	}
} // namespace cave
#endif