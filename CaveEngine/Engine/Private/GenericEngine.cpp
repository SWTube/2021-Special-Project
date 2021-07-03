/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "GenericEngine.h"

namespace cave
{
	GenericEngine::GenericEngine()
		: mPool(&gCoreMemoryPool)
	{
	}

	const wchar_t* GenericEngine::msWindowClassName = L"GenericWindowClass";

	GenericEngine::~GenericEngine()
	{
		if (mRenderer != nullptr)
		{
			mRenderer->~Renderer();
			mPool->Deallocate(mRenderer, sizeof(Renderer));
			mRenderer = nullptr;
		}

		if (mWindow != nullptr)
		{
			mWindow->~Window();
			mPool->Deallocate(mWindow, sizeof(Window));
			mWindow = nullptr;
		}

		if (mPool != &gCoreMemoryPool)
		{
			delete mPool;
		}
	}

	Window* GenericEngine::GetWindowHandle()
	{
		return mWindow;
	}

	Renderer* GenericEngine::GetRenderer()
	{
		return mRenderer;
	}
}
