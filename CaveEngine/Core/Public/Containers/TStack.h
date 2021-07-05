/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#include "CoreTypes.h"

// #include "Containers/Array.h"
#include "Memory/Memory.h"
#include "Memory/MemoryPool.h"

namespace cave
{
	/**
	 *
	 * @brief TStack is a LIFO data structure.
	 * @details The cave::TStack class is a container adapter that gives the programmer the functionality of a stack - specifically, 
	 * 			a LIFO (last-in, first-out) data structure.@n
	 * 			The class template acts as a wrapper to the underlying container - only a specific set of functions is provided.@n 
	 * 			The stack pushes and pops the element from the back of the underlying container, known as the top of the stack. 
	 * @author Alegruz
	 * @date 2021-06-02
	 * @version 0.0.1
	 *
	 */
	template <typename T>
	class TStack final
	{
		// Non-member functions
		template <typename U>
		friend constexpr bool operator==(const TStack<U>& lhs, const TStack<U>& rhs);
		template <typename U>
		friend constexpr bool operator!=(const TStack<U>& lhs, const TStack<U>& rhs);
	public:
		TStack();
		TStack(MemoryPool& pool);
		TStack(const TStack& other);
		TStack(const TStack& other, MemoryPool& pool);
		TStack(TStack&& other);
		TStack(TStack&& other, MemoryPool& pool);
		~TStack();
		TStack& operator=(const TStack& other);
		TStack& operator=(TStack&& other);

		// Element Access
		T& GetTop();
		const T& GetTop() const;

		// Capacity
		[[nodiscard]] bool IsEmpty() const;
		size_t GetSize() const;

		// Modifiers
		void Push(const T& value);
		void Push(T&& value);
		void Pop();

		// Constants
		static constexpr size_t INITIAL_CAPACITY = 8ul;
		static constexpr size_t ALIGNED_BYTE = 16ul;
	private:
		MemoryPool* mPool = nullptr;
		size_t mSize = 0u;
		size_t mCapacity = INITIAL_CAPACITY;
		T* mArray = nullptr;
	};

	/**
	 *
  	 * @brief (1) Constructs the stack
	 * @details Default constructor. Value-initializes the container.
	 * 			@n@n
	 * 			Complexity: constant
	 *
	 */
	template <typename T>
	TStack<T>::TStack()
		: TStack(gCoreMemoryPool)
	{	
	}

	/**
	 *
  	 * @brief (2) Constructs the stack
	 * @details Constructs the underlying container using pool as allocator.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pool memory pool to use for all memory allocations of the underlying container 
	 *
	 */
	template <typename T>
	TStack<T>::TStack(MemoryPool& pool)
		: mPool(&pool)
		, mArray(reinterpret_cast<T*>(mPool->Allocate(sizeof(T) * mCapacity)))
	{
	}

	/**
	 *
  	 * @brief (3) Constructs the stack
	 * @details Copy constructor. The adaptor is copy-constructed with the contents of other.mArray.
	 * 			@n@n
	 * 			Complexity: linear in size of other.mArray
	 * @param other another container adaptor to be used as source to initialize the underlying container
	 *
	 */
	template <typename T>
	TStack<T>::TStack(const TStack& other)
		: TStack(other, *other.mPool)
	{
	}
	
	/**
	 *
  	 * @brief (4) Constructs the stack
	 * @details Constructs the adaptor with the contents of other.mArray and using pool as allocator.
	 * 			@n@n
	 * 			Complexity: linear in size of other.mArray
	 * @param pool memory pool to use for all memory allocations of the underlying container 
	 * @param other another container adaptor to be used as source to initialize the underlying container
	 *
	 */
	template <typename T>
	TStack<T>::TStack(const TStack& other, MemoryPool& pool)
		: mPool(&pool)
		, mSize(other.mSize)
		, mCapacity(other.mCapacity)
		, mArray(reinterpret_cast<T*>(mPool->Allocate(sizeof(T) * mCapacity)))
	{
		Memory::Memcpy(mArray, other.mArray, mSize);
	}
	
	/**
	 *
  	 * @brief (5) Constructs the stack
	 * @details Move constructor. The adaptor is constructed with std::move(other.mArray)
	 * 			@n@n
	 * 			Complexity: constant
	 * @param other another container adaptor to be used as source to initialize the underlying container
	 *
	 */
	template <typename T>
	TStack<T>::TStack(TStack&& other)
		: TStack(std::move(other), *other.mPool)
	{
	}
	
	/**
	 *
  	 * @brief (6) Constructs the stack
	 * @details Constructs the adaptor with the contents of other using move semantics while utilizing pool as allocator.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pool memory pool to use for all memory allocations of the underlying container 
	 * @param other another container adaptor to be used as source to initialize the underlying container
	 *
	 */
	template <typename T>
	TStack<T>::TStack(TStack&& other, MemoryPool& pool)
		: mPool(&pool)
		, mSize(other.mSize)
		, mCapacity(other.mCapacity)
		, mArray(other.mArray)
	{
		other.mSize = 0ul;
		other.mCapacity = 0ul;
		other.mArray = nullptr;
	}

	/**
	 *
  	 * @brief Destructs the stack.
	 * @details The destructors of the elements are called and the used storage is deallocated. @n
	 * 			Note, that if the elements are pointers, the pointed-to objects are not destroyed.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pool memory pool to use for all memory allocations of the underlying container 
	 * @param other another container adaptor to be used as source to initialize the underlying container
	 *
	 */
	template <typename T>
	TStack<T>::~TStack()
	{
		for (uint32_t i = 0; i < mSize; ++i)
		{
			mArray[i].~T();
		}
		mPool->Deallocate(mPool, sizeof(T) * mCapacity);
		mSize = 0ul;
		mCapacity = 0ul;
		mPool = nullptr;
	}

	/**
	 *
  	 * @brief (1) Assigns values to the container adaptor
	 * @details Replaces the contents of the container adaptor with those of other. @n
	 * 			Copy assignment operator. Replaces the contents with a copy of the contents of other. Effectively calls mArray = other.mArray;
	 * 			@n@n
	 * 			Complexity: equivalent to that of `operator=` of the underlying container
	 * @param other another container adaptor to be used as source
	 * @return *this
	 *
	 */
	template <typename T>
	TStack<T>& TStack<T>::operator=(const TStack& other)
	{
		if (this != &other)
		{
			if (mCapacity < other.mSize)
			{
				size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(other.mSize);
				T* newArray = reinterpret_cast<T*>(mPool->Allocate(sizeof(T) * newCapacity));
				mPool->Deallocate(mArray, sizeof(T) * mCapacity);
				mArray = newArray;
				mCapacity = newCapacity;
			}

			Memory::Memcpy(mArray, other.mArray, other.mSize);
			mSize = other.mSize;
		}

		return *this;
	}

	/**
	 *
  	 * @brief (2) Assigns values to the container adaptor
	 * @details Replaces the contents of the container adaptor with those of other. @n
	 * 			Move assignment operator. Replaces the contents with those of other using move semantics. Effectively calls mArray = std::move(other.mArray);
	 * 			@n@n
	 * 			Complexity: equivalent to that of `operator=` of the underlying container
	 * @param other another container adaptor to be used as source
	 * @return *this
	 *
	 */
	template <typename T>
	TStack<T>& TStack<T>::operator=(TStack&& other)
	{
		if (this != &other)
		{
			mPool->Deallocate(mArray, mCapacity);

			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mArray = other.mArray;

			other.mSize = 0ul;
			other.mCapacity = 0ul;
			other.mArray = nullptr;
		}

		return *this;
	}

	/**
	 *
  	 * @brief (1) Accesses the top element
	 * @details Returns reference to the top element in the stack. @n
	 * 			This is the most recently pushed element. @n
	 * 			This element will be removed on a call to Pop().
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the last element
	 *
	 */
	template <typename T>
	T& TStack<T>::GetTop()
	{
		return mArray[mSize - 1ul];
	}

	/**
	 *
  	 * @brief (2) Accesses the top element
	 * @details Returns reference to the top element in the stack. @n
	 * 			This is the most recently pushed element. @n
	 * 			This element will be removed on a call to Pop().
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the last element
	 *
	 */
	template <typename T>
	const T& TStack<T>::GetTop() const
	{
		return mArray[mSize - 1ul];
	}

	/**
	 *
  	 * @brief Checks whether the underlying container is empty
	 * @details Checks if the underlying container has no elements, i.e. whether c.IsEmpty(). 
	 * 			@n@n
	 * 			Complexity: constant
	 * @return `true` if the underlying container is empty, `false` otherwise 
	 *
	 */
	template <typename T>
	[[nodiscard]] bool TStack<T>::IsEmpty() const
	{
		return mSize == 0ul;
	}

	/**
	 *
  	 * @brief Returns the number of elements
	 * @details Returns the number of elements in the underlying container, that is, c.GetSize().
	 * 			@n@n
	 * 			Complexity: constant
	 * @return The number of elements in the container.
	 *
	 */
	template <typename T>
	size_t TStack<T>::GetSize() const
	{
		return mSize;
	}

	/**
	 *
  	 * @brief (1) Inserts element at the top
	 * @details Pushes the given element value to the top of the stack. @n
	 * 			Effectively calls c.PushBack(value)
	 * 			@n@n
	 * 			Complexity: Equal to the complexity of Container::PushBack
	 * @param value the value of the element to push
	 *
	 */
	template <typename T>
	void TStack<T>::Push(const T& value)
	{
		if (mSize >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mSize + 1);
#endif
			T* newArray = reinterpret_cast<T*>(mPool->Allocate(sizeof(T) * newCapacity));
			Memory::Memcpy(newArray, mArray, mSize);
			mPool->Deallocate(mArray, sizeof(T) * mCapacity);
			mArray = newArray;
		}

		mArray[mSize++] = value;
	}

	/**
	 *
  	 * @brief (2) Inserts element at the top
	 * @details Pushes the given element value to the top of the stack. @n
	 * 			Effectively calls c.PushBack(value)
	 * 			@n@n
	 * 			Complexity: Equal to the complexity of Container::PushBack
	 * @param value the value of the element to push
	 *
	 */
	template <typename T>
	void TStack<T>::Push(T&& value)
	{
		if (mSize >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mSize + 1);
#endif
			T* newArray = reinterpret_cast<T*>(mPool->Allocate(sizeof(T) * newCapacity));
			Memory::Memcpy(newArray, mArray, mSize);
			mPool->Deallocate(mArray, sizeof(T) * mCapacity);
			mArray = newArray;
		}

		mArray[mSize++] = std::move(value);
		value.~T();
	}

	/**
	 *
  	 * @brief Removes the top element
	 * @details Removes the top element from the stack. @n
	 * 			Effectively calls c.PopBack()
	 * 			@n@n
	 * 			Complexity: Equal to the complexity of Container::PopBack
	 *
	 */
	template <typename T>
	void TStack<T>::Pop()
	{
		if (mSize > 0ul)
		{
			mArray[mSize - 1].~T();
			--mSize;
		}
	}

	/**
	 *
  	 * @brief (1) Compares the values in the stack
	 * @details Compares the contents of the underlying containers of two container adaptors. @n
	 * 			The comparison is done by applying the corresponding operator to the underlying containers. 
	 * 			@n@n
	 * 			Complexity: Linear in the size of the container
	 * @param lhs container adaptors whose contents to compare
	 * @param rhs container adaptors whose contents to compare
	 * @return `true` if the corresponding comparison yields `true`, `false` otherwise
	 *
	 */
	template <typename T>
	constexpr bool operator==(const TStack<T>& lhs, const TStack<T>& rhs)
	{
		if (lhs.mSize != rhs.mSize)
		{
			return false;
		}

		return Memory::Memcmp(lhs.mArray, rhs.mArray, lhs.mSize) == 0;
	}

	/**
	 *
  	 * @brief (2) Compares the values in the stack
	 * @details Compares the contents of the underlying containers of two container adaptors. @n
	 * 			The comparison is done by applying the corresponding operator to the underlying containers. 
	 * 			@n@n
	 * 			Complexity: Linear in the size of the container
	 * @param lhs container adaptors whose contents to compare
	 * @param rhs container adaptors whose contents to compare
	 * @return `true` if the corresponding comparison yields `true`, `false` otherwise
	 *
	 */
	template <typename T>
	constexpr bool operator!=(const TStack<T>& lhs, const TStack<T>& rhs)
	{
		if (lhs.mSize != rhs.mSize)
		{
			return true;
		}

		return Memory::Memcmp(lhs.mArray, rhs.mArray, lhs.mSize) != 0;
	}

#ifdef CAVE_BUILD_DEBUG
	namespace StackTest
	{
		// DECLARATIONS
		template <typename T>
		void Test();

		template <typename T>
		void Constructor();

		template <typename T>
		void AssignmentOperator();

		template <typename T>
		void GetTop();

		template <typename T>
		void IsEmpty();

		template <typename T>
		void GetSize();

		template <typename T>
		void Push();

		template <typename T>
		void Pop();

		template <typename T>
		void Swap();

		template <typename T>
		void ComparisonOperator();

		// DEFINITIONS

		template <typename T>
		void Test()
		{
			LOGD(eLogChannel::CORE_CONTAINER, "======Stack Test======");
			Constructor<T>();
			// AssignmentOperator<T>();
			// GetTop<T>();
			// IsEmpty<T>();
			// GetSize<T>();
			// Push<T>();
			// Pop<T>();
			// Swap<T>();
			// ComparisonOperator<T>();
		}

		template <typename T>
		void Constructor()
		{
			LOGD(eLogChannel::CORE_CONTAINER, "====Constructor Test====");

			{
				TStack<T> stack;
				
				LOGD(eLogChannel::CORE_CONTAINER, "Constructor #1 Success");
			}
		}

		// template <typename T>
		// void AssignmentOperator();

		// template <typename T>
		// void GetTop();

		// template <typename T>
		// void IsEmpty();

		// template <typename T>
		// void GetSize();

		// template <typename T>
		// void Push();

		// template <typename T>
		// void Pop();

		// template <typename T>
		// void Swap();

		// template <typename T>
		// void ComparisonOperator();
	}
#endif
} // namespace cave
