/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "GraphicsApiPch.h"

#include "CoreMinimal.h"
#include "Device/DeviceResources.h"
#include "Sprite/Vertex.h"
#include "Renderer/GenericRenderer.h"

#define BUFFER_OFFSET(a) (reinterpret_cast<void*>(a))
#define FAILED(error) ((static_cast<int32_t>(error)) != GLFW_NO_ERROR)

#ifdef __UNIX__
namespace cave
{
	typedef struct
	{
		GLenum       type;
		const char*  filename;
		GLuint       shader;
	} ShaderInfo;

	class UnixRenderer final : public GenericRenderer
	{
	public:
		UnixRenderer() = default;
		virtual ~UnixRenderer();

		UnixRenderer(const UnixRenderer&) = delete;
		UnixRenderer& operator=(const UnixRenderer&) = delete;
		UnixRenderer& operator=(const UnixRenderer&&) = delete;

		bool WindowShouldClose() override;

		virtual eResult Init(Window* window) override;
		eResult CreateDeviceDependentResources() override;
		eResult CreateWindowSizeDependentResources(Window* window = nullptr) override;
		void Update() override;
		// Render steps:
			// 1. Clear the window by calling glClearBufferfv()
			// 2. Issue the OpenGL calls required to render your object
			// 3. Request that the image is presented to the screen
		void Render() override;

		void Destroy() override;

	private:
		void cleanupDevice();

		std::vector<uint32_t> mVertexArrayObjects;
		// GLuint msTextures[TEXTURE_COUNT];
	};

	typedef UnixRenderer Renderer;
}
#endif