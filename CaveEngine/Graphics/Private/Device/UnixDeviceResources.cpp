/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#ifdef __UNIX__
#include "Device/UnixDeviceResources.h"

#include "String/String.h"

namespace cave
{
	UnixDeviceResources::UnixDeviceResources(MemoryPool& pool)
		: GenericDeviceResources(pool)
	{
	}
	
	eResult UnixDeviceResources::Init(Window* window)
	{
		eResult result = CreateDeviceResources();
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		result = CreateWindowResources(window);
		if (result != eResult::CAVE_OK)
		{
			return result;
		}

		return result;
	}

	eResult UnixDeviceResources::CreateDeviceResources()
	{
		glfwSetErrorCallback(errorCallback);

		int32_t glfwInitResult = glfwInit();
		if (glfwInitResult == GLFW_FALSE)
		{
			glfwTerminate();
			return eResult::CAVE_FAIL;
		}

		return eResult::CAVE_OK;
	}

	eResult UnixDeviceResources::CreateWindowResources(Window* window)
	{
		int32_t result = GLFW_NO_ERROR;

		assert(window != nullptr);
		mWindow = window;
		mWidth = mWindow->GetWidth();
		mHeight = mWindow->GetHeight();
		GLFWwindow* glfwWindow = mWindow->GetWindow();

		// 2. Make Context Current ---------------------------------------------------------------------------------------------
		glfwMakeContextCurrent(glfwWindow);
		if (result = glfwGetError(nullptr); result != GLFW_NO_ERROR)
		{
			LOGEF(eLogChannel::GRAPHICS, "glfwMakeContextCurrent error code: 0x%x", result);
		}
		gl3wInit();

		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		size_t vendorStringSize = Strlen(vendor);
		const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		size_t rendererStringSize = Strlen(renderer);
		Strcpy(mVideoCardDescription, 128ul, vendor, vendorStringSize);
		mVideoCardDescription[vendorStringSize] = ' ';
		Strcpy(&mVideoCardDescription[vendorStringSize + 1], 128ul - vendorStringSize - 1, renderer, Strlen(renderer));
		Memory::Memset(&mVideoCardDescription[vendorStringSize + 1 + rendererStringSize], 0, 128 - vendorStringSize - rendererStringSize - 1);

		LOGIF(eLogChannel::GRAPHICS, "Vendor: %s", glGetString(GL_VENDOR));
		LOGIF(eLogChannel::GRAPHICS, "Renderer: %s", glGetString(GL_RENDERER));
		LOGIF(eLogChannel::GRAPHICS, "OpenGL version supported: %s", glGetString(GL_VERSION));
		int32_t major = 0;
		int32_t minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		LOGIF(eLogChannel::GRAPHICS, "OpenGL version supported: %d", major);
		LOGIF(eLogChannel::GRAPHICS, "OpenGL version supported: %d", minor);

		glViewport(0, 0, mWidth, mHeight);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ConfigureBackBuffer();

		return eResult::CAVE_OK;
	}

	int32_t UnixDeviceResources::ConfigureBackBuffer()
	{
		int32_t result = GLFW_NO_ERROR;

		// 투영 행렬을 설정합니다
		float fieldOfView = glm::quarter_pi<float>();
		float screenAspect = static_cast<float>(mWidth) / static_cast<float>(mHeight);

		// 3D 렌더링을위한 투영 행렬을 만듭니다
		mProjection = glm::perspectiveRH(fieldOfView, screenAspect, mWindow->GetNear(), mWindow->GetFar());

		// 세계 행렬을 항등 행렬로 초기화합니다
		mWorld = glm::mat4(1.0f);

		// 2D 렌더링을위한 직교 투영 행렬을 만듭니다
		// mOrtho = glm::orthoRH(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, mWindow->GetNear(), mWindow->GetFar());
		mOrtho = glm::orthoRH(-static_cast<float>(mWidth) * 0.5f, static_cast<float>(mWidth), static_cast<float>(mWidth) * 0.5f, -static_cast<float>(mHeight) * 0.5f, mWindow->GetNear(), mWindow->GetFar());

		return result;
	}

	int32_t UnixDeviceResources::ReleaseBackBuffer()
	{
		int32_t result = GLFW_NO_ERROR;

		return result;
	}

	int32_t UnixDeviceResources::GoFullScreen()
	{
		int32_t result = GLFW_NO_ERROR;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(mWindow->GetWindow(), glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		if (mWindow->GetWindow() == nullptr)
		{
			return glfwGetError(nullptr);
		}

		return result;
	}
	
	int32_t UnixDeviceResources::GoWindowed()
	{
		int32_t result = GLFW_NO_ERROR;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(mWindow->GetWindow(), nullptr, 0, 0, static_cast<int32_t>(mWidth), static_cast<int32_t>(mHeight), mode->refreshRate);
		if (mWindow->GetWindow() == nullptr)
		{
			return glfwGetError(nullptr);
		}

		return result;
	}

	float UnixDeviceResources::GetAspectRatio()
	{
		return static_cast<float>(mWidth) / static_cast<float>(mHeight);
	}

	void UnixDeviceResources::RenderStart()
	{
		// 2. Clear buffer ---------------------------------------------------------------------------------------------
		static const float midnightBlue[] = { 0.098039225f, 0.098039225f, 0.439215720f, 0.000000000f };

		glClearBufferfv(GL_COLOR, 0, midnightBlue);
	}

	void UnixDeviceResources::RenderEnd()
	{
		// 6. Swap buffers ---------------------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow->GetWindow());
	}

	void UnixDeviceResources::GetVideoCardInfo(char* cardName, int& memory)
	{
		Strcpy(cardName, 128ul, mVideoCardDescription, 128ul);
		memory = mVideoCardMemory;
	}

	void UnixDeviceResources::TurnZBufferOn()
	{
	}


	void UnixDeviceResources::TurnZBufferOff()
	{
	}

	void UnixDeviceResources::errorCallback(int32_t errorCode, const char* description)
	{
		LOGEF(eLogChannel::GRAPHICS, "%s, errorCode: %d", description, errorCode);
	}

	void UnixDeviceResources::Destroy()
	{
		if (mWindow->GetWindow() != nullptr)
		{
			glfwDestroyWindow(mWindow->GetWindow());
		}

		glfwTerminate();
	}

	uint32_t UnixDeviceResources::GetProgram() const
	{
		return mProgram;
	}

	void UnixDeviceResources::SetProgram(uint32_t program)
	{
		mProgram = program;
	}

	uint32_t UnixDeviceResources::GetWidth() const
	{
		return mWidth;
	}

	uint32_t UnixDeviceResources::GetHeight() const
	{
		return mHeight;
	}
} // namespace cave
#endif