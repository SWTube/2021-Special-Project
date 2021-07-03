/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "UnixEngine.h"

#ifdef __UNIX__
namespace cave
{
	eResult UnixEngine::Init(uint32_t screenWidth, uint32_t screenHeight)
	{
		eResult result = eResult::CAVE_OK;

		// Instantiate the renderer.
		mRenderer = reinterpret_cast<Renderer*>(mPool->Allocate(sizeof(Renderer)));
		new(mRenderer) Renderer();
		mRenderer->CreateDeviceDependentResources();

		mWindow = reinterpret_cast<Window*>(mPool->Allocate(sizeof(Window)));
		new(mWindow) Window(screenWidth, screenHeight, "Test", nullptr, nullptr);
		
		result = mRenderer->CreateWindowSizeDependentResources(mWindow);
		if (result != eResult::CAVE_OK)
		{
			return result;
		}
		
		return result;
	}

	void UnixEngine::Destroy()
	{
	}


	eResult UnixEngine::Run()
	{
		long double tic = 0.0l;
		long double toc = 0.0l;
		long double tick = 0.0l;
		struct timespec tp;
		long double startTic = 0.0l;
		bool bHasSecondPassed = false;
		uint32_t tickCount = 0u;
		while (!mRenderer->WindowShouldClose())
		{
			clock_gettime(CLOCK_MONOTONIC, &tp);
			tic = (static_cast<long double>(tp.tv_sec) * 1000.0l) + (static_cast<long double>(tp.tv_nsec) * 0.000001l);
			if (bHasSecondPassed)
			{
				startTic = tic;
				bHasSecondPassed = false;
				LOGIF(cave::eLogChannel::GRAPHICS, std::cout, "FPS: %u", static_cast<uint32_t>(1.0l / ((tick / static_cast<long double>(tickCount)) * 0.001l)));
				tick = 0.0l;
				tickCount = 0u;
			}
			// Update the scene.
			mRenderer->Update();

			// Render frames during idle time (when no messages are waiting).
			mRenderer->Render();

			glfwPollEvents();
			clock_gettime(CLOCK_MONOTONIC, &tp);
			toc = (static_cast<long double>(tp.tv_sec) * 1000.0l) + (static_cast<long double>(tp.tv_nsec) * 0.000001l);
			tick += toc - tic;
			++tickCount;
			if (toc - startTic >= 1000.0l)
			{
				bHasSecondPassed = true;
			}
		}

		return eResult::CAVE_OK;
	}
}
#endif