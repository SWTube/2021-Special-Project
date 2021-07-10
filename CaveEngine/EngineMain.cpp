/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

//#if defined(__WIN32__)
//import Log;
//
////--------------------------------------------------------------------------------------
//// Entry point to the program. Initializes everything and goes into a message processing 
//// loop. Idle time is used to render the scene.
////--------------------------------------------------------------------------------------
//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	// Enable run-time memory check for debug builds.
//#if defined(CAVE_BUILD_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//#else
//int main(int32_t argc, char** argv)
//{
//	std::cout << "Hello World!" << "\n";
//#endif
//
//	// RenderTest();
//#ifdef CAVE_BUILD_DEBUG
//	cave::MemoryPoolTest::Test();
//	// cave::StackTest::Test<int>();
//#endif
//
//	// Cleanup is handled in destructors.
//	return 0;
//}

#include <iostream>

import Matrix;

using namespace cave;

int main()
{
	std::cout << "Hello World!\n";
}