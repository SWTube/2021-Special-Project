/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include "Containers/Node.h"
#include "Memory/Memory.h"

namespace cave
{
	Node<void*>::Node(void* data, Node* next)
		: Data(data)
		, Next(next)
	{
	}

	Node<void*>::~Node()
	{
		if (Data != nullptr)
		{
			Memory::Free(Data);
		}
	}
}