/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

namespace cave
{
	template <typename T>
	struct Node final
	{
		T Data;
		Node* Next = nullptr;

		Node() = delete;
		Node(T data, Node* next);
		Node(const Node&) = delete;
		Node(const Node&&) = delete;
		Node& operator=(const Node&) = delete;
		~Node();
	};

	template <typename T>
	Node<T>::Node(T data, Node* next)
		: Data(data)
		, Next(next)
	{
	}

	template <typename T>
	Node<T>::~Node()
	{
		if (Data != nullptr)
		{
			delete Data;
		}
	}

	template <>
	struct Node<void*> final
	{
		void* Data = nullptr;
		Node<void*>* Next = nullptr;

		Node() = delete;
		Node(void* data, Node* next);
		Node(const Node&) = delete;
		Node(const Node&&) = delete;
		Node& operator=(const Node&) = delete;
		~Node();
	};

	typedef Node<void*> MemoryNode;
}