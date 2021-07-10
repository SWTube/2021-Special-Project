/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __UNIX__
#include "Shader/GenericShader.h"

	namespace cave
	{
		class UnixShader final : public GenericShader
		{
		public:
			UnixShader() = delete;
			UnixShader(const std::filesystem::path& vertexShaderFilePath, const std::filesystem::path& fragmentShaderFilePath, MemoryPool& pool);
			UnixShader(const UnixShader&) = delete;
			UnixShader(UnixShader&& other);
			UnixShader& operator=(const UnixShader&) = delete;
			UnixShader& operator=(UnixShader&& other);
			virtual ~UnixShader() = default;

			virtual void Render(uint32_t indexCount, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, uint32_t texture) override;
			virtual eResult Compile() override;
			virtual eResult SetInputLayout(const Sprite& sprite) override;
			virtual uint32_t GetProgram();
		private:
			typedef struct
			{
				GLenum       type;
				const char*  filename;
				GLuint       shader;
			} ShaderInfo;

			eResult compileShaderFromFile(ShaderInfo* shaders);
			uint32_t loadShaders(ShaderInfo* shaders);
			const char* readShader(const char* filename);

			static constexpr uint32_t V_POSITION = 0u;
			static constexpr uint32_t V_TEX_COORD = 1u;

			// uint32_t mVertexArrayObject = 0u;
			uint32_t mProgram = 0u;
		};

		typedef UnixShader Shader;
	} // namespace cave
#endif