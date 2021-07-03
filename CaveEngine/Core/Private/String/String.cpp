/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#include <sstream>

#include "String/String.h"

#ifdef __UNIX__
namespace cave
{
	template <size_t N>
	FORCEINLINE constexpr size_t GetSufficientCapacity(size_t length)
	{
		assert(length <= SIZE_MAX - 1ul - N);

		return (length + 1ul + N - 1ul) - ((length + 1ul + N - 1ul) % N);
	}
	// Constructor

	/**
	 *
  	 * @brief (1) Default constructor.
	 * @details Constructs empty string (zero size and zero capacity). Memory pool is obtained from the Core Memory Pool.
	 * 			@n@n
	 * 			Complexity: constant
	 *
	 */
	String::String() noexcept(noexcept(gCoreMemoryPool))
		: String(gCoreMemoryPool)
	{
	}

	/**
	 *
  	 * @brief (1) Default constructor.
	 * @details Constructs empty string (zero size and zero capacity). Memory pool is obtained from the Core Memory Pool.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	String::String(MemoryPool& pool) noexcept
		: mPool(&pool)
		, mLength(0ul)
		, mCapacity(ALIGNED_BYTE)
		, mString(static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char))))
	{
		Memory::Memset(mString, '\0', mCapacity);
	}

	/**
	 *
  	 * @brief (2) Constructs the string with count copies of character ch.
	 * @details Complexity: linear in count
	 * @param count size of the resulting string
	 * @param ch value to initialize the string with
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	String::String(size_t count, char ch, MemoryPool& pool)
		: mPool(&pool)
		, mLength(count)
		, mCapacity(GetSufficientCapacity<ALIGNED_BYTE>(mLength))
		, mString(static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char))))
	{
		Memory::Memset(mString, ch, mLength);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (3) Constructs the string with a substring [pos, pos+count) of other.
	 * @details If count == NPOS, if count is not specified, or if the requested substring lasts past the end of the string,
	 * 			the resulting substring is [pos, other.GetLength()).
	 * 			@n@n
	 * 			Complexity: linear in count
	 * @param other another string to use as source to intialize the string with
	 * @param pos position of the first character to include
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	String::String(const String& other, size_t pos, MemoryPool& pool)
		: String(other, pos, NPOS, pool)
	{
	}

	/**
	 *
  	 * @brief (3) Constructs the string with a substring [pos, pos+count) of other.
	 * @details If count == NPOS, if count is not specified, or if the requested substring lasts past the end of the string,
	 * 			the resulting substring is [pos, other.GetLength()).
	 * 			@n@n
	 * 			Complexity: linear in count
	 * @param other another string to use as source to intialize the string with
	 * @param pos position of the first character to include
	 * @param count size of the resulting string
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	String::String(const String& other, size_t pos, size_t count, MemoryPool& pool)
		: mPool(&pool)
		, mLength(count)
		, mCapacity(other.mCapacity)
		, mString(static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char))))
	{
		assert(other.mString != nullptr);
		
		if (count == NPOS || other.mLength < count)
		{
			count = other.mLength;
		}
		mLength = count;

		// Copy s to mString
		Strcpy(mString, mCapacity, &other.mString[pos], count);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}
	
	/**
	 *
  	 * @brief (4) Constructs the string with the first count characters of character string pointed to by s.
	 * @details s can contain null characters.
	 * 			The length of the string is count.
	 * 			The behavior is undefined if [s, s + count) is not a valid range.
	 * 			@n@n
	 * 			Complexity: linear in count
	 * @param s	pointer to an array of characters to use as source to initialize the string with 
	 * @param count size of the resulting string
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	constexpr String::String(const char* s, size_t count, MemoryPool& pool)
		: mPool(&pool)
	{
		assert(s != nullptr);

		mLength = count;
		mCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mLength);
		mString = static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char)));

		size_t sLength = Strlen(s);
		if (count == NPOS || sLength < count)
		{
			count = sLength;
		}

		// Copy s to mString
		Strcpy(mString, mCapacity, s, count);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (5) Constructs the string with the contents initialized with a copy of the null-terminated character string pointed to by s.
	 * @details The length of the string is determined by the first null character.
	 * 			The behavior is undefined if [s, s + Strlen(s)) is not a valid range (for example, if s is a null pointer).
	 * 			@n@n
	 * 			Complexity: linear in length of s
	 * @param s	pointer to an array of characters to use as source to initialize the string with 
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	constexpr String::String(const char* s, MemoryPool& pool)
		: mPool(&pool)
	{
		assert (s != nullptr);

		// Allocate Memory
		mLength = Strlen(s);
		mCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mLength);
		mString = static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char)));

		// Copy s to mString
		Strcpy(mString, mCapacity, s, mLength);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (6) Constructs the string with the contents of the range [first, last).
	 * @details Complexity: linear in distance between first and last
	 * @param first	range to copy the characters from
	 * @param last range to copy the characters from
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	constexpr String::String(const char* first, const char* last, MemoryPool& pool)
		: mPool(&pool)
	{
		assert(first != nullptr && last != nullptr);

		// Allocate Memory
		const char* temp = first;
		for (; temp != last; ++temp, ++mLength)
		{
		}

		mCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mLength);
		mString = static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char)));

		memcpy(mString, first, mLength);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (7) Copy constructor.
	 * @details Constructs the string with a copy of the contents of other.
	 * 			@n@n
	 * 			Complexity: linear in size of other
	 * @param other	another string to use as source to initialize the string with
	 *
	 */
	constexpr String::String(const String& other)
		: mPool(other.mPool)
		, mLength(other.mLength)
		, mCapacity(other.mCapacity)
	{
		assert(other.mPool != nullptr && other.mString != nullptr);
		mString = static_cast<char*>(mPool->Allocate((mCapacity) * sizeof(char)));
		Strcpy(mString, mCapacity, other.mString, mLength);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (7) Copy constructor.
	 * @details Constructs the string with a copy of the contents of other.
	 * 			@n@n
	 * 			Complexity: linear in size of other
	 * @param other	another string to use as source to initialize the string with
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	constexpr String::String(const String& other, MemoryPool& pool)
		: mPool(&pool)
		, mLength(other.mLength)
		, mCapacity(other.mCapacity)
	{
		assert(other.mString != nullptr);
		mString = static_cast<char*>(mPool->Allocate((mCapacity) * sizeof(char)));
		Strcpy(mString, mCapacity, other.mString, mLength);
		Memory::Memset(&mString[mLength], '\0', mCapacity - mLength);
	}

	/**
	 *
  	 * @brief (8) Move constructor.
	 * @details Constructs the string with the contents of other using move semantics.
	 * 			other is left in valid, but unspecified state.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param other	another string to use as source to initialize the string with
	 *
	 */
	constexpr String::String(String&& other) noexcept
		: mPool(other.mPool)
		, mLength(other.mLength)
		, mCapacity(other.mCapacity)
		, mString(other.mString)
	{
		assert(other.mPool != nullptr && other.mString != nullptr);

		other.mPool = nullptr;
		other.mLength = 0ul;
		other.mCapacity = 0ul;
		other.mString = nullptr;

		other.~String();
	}

	/**
	 *
  	 * @brief (8) Move constructor.
	 * @details Constructs the string with the contents of other using move semantics.
	 * 			other is left in valid, but unspecified state.
	 * 			@n@n
	 * 			Complexity: constant. If <code>pool</pool> is given and <code>pool != other.pool</code>, then linear
	 * @param other	another string to use as source to initialize the string with
	 * @param pool memory pool to use for all memory allocations of this string
	 *
	 */
	constexpr String::String(String&& other, MemoryPool& pool)
		: mPool(&pool)
		, mLength(other.mLength)
		, mCapacity(other.mCapacity)
		, mString(other.mString)
	{
		assert(other.mString != nullptr);

		other.mPool = nullptr;
		other.mLength = 0ul;
		other.mCapacity = 0ul;
		other.mString = nullptr;

		other.~String();
	}

	/**
	 *
  	 * @brief destroys the string, deallocating internal storage if used
	 *
	 */
	String::~String()
	{
		if (mString != nullptr)
		{
			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = nullptr;
		}

		if (mPool != nullptr)
		{
			mPool = nullptr;
		}
	}

	/**
	 *
  	 * @brief (1) Assigns values to the string 
	 * @details Replaces the contents with a copy of <code>str</code>. If *this and str are the same object, this function has no effect.
	 * 			@n@n
	 * 			Complexity: linear in size of <code>str</code>
	 * @param str string to be used as source to initialize the string with
	 * @return *this
	 *
	 */
	String& String::operator=(const String& str)
	{
		if (this != &str)
		{
			assert(mString != str.mString && str.mString != nullptr);

			if (mCapacity < str.mLength + 1)
			{
				mPool->Deallocate(mString, mCapacity * sizeof(char));

				mCapacity = GetSufficientCapacity<ALIGNED_BYTE>(str.mLength);

				mString = static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char)));
			}

			mLength = str.mLength;
			Memory::Memset(mString, '\0', mLength);
			Strcpy(mString, mCapacity, str.mString, mLength);
		}

		return *this;
	}

	/**
	 *
  	 * @brief (2) Assigns values to the string 
	 * @details Replaces the contents with those of str using move semantics. 
	 * 			str is in a valid but unspecified state afterwards. 
	 * 			Unlike other container move assignments, references, pointers, and iterators to str may be invalidated.
	 * 			@n@n
	 * 			Complexity: linear in size of <code>this</code>
	 * @param str string to be used as source to initialize the string with
	 * @return *this
	 *
	 */
	String& String::operator=(String&& str)
	{
		if (this != &str)
		{
			assert(mString != str.mString && str.mString != nullptr);

			if (mString != nullptr)
			{
				mPool->Deallocate(mString, mCapacity * sizeof(char));
			}

			mLength = str.mLength;
			mCapacity = str.mCapacity;
			mString = str.mString;

			str.mPool = nullptr;
			str.mLength = 0ul;
			str.mCapacity = 0ul;
			str.mString = nullptr;

			str.~String();
		}

		return *this;
	}

	/**
	 *
  	 * @brief (3) Assigns values to the string 
	 * @details Replaces the contents with those of null-terminated character string pointed to by s.
	 * 			@n@n
	 * 			Complexity: linear in size of <code>s</code>
	 * @param s pointer to a null-terminated character string to use as source to initialize the string with
	 * @return *this
	 *
	 */
	String& String::operator=(const char* s)
	{
		assert(s != nullptr);

		if (s != mString)
		{
			size_t sLength = Strlen(s);

			if (mCapacity < sLength + 1)
			{
				mPool->Deallocate(mString, mCapacity * sizeof(char));

				mCapacity = GetSufficientCapacity<ALIGNED_BYTE>(sLength);

				mString = static_cast<char*>(mPool->Allocate(mCapacity * sizeof(char)));
			}

			Memory::Memset(mString, '\0', mLength);

			mLength = sLength;
			Strcpy(mString, mCapacity, s, mLength);
		}

		return *this;
	}

	/**
	 *
  	 * @brief (4) Assigns values to the string 
	 * @details Replaces the contents with character ch.
	 * 			@n@n
	 * 			Complexity: linear in size of <code>str</code>
	 * @param ch value to initialize characters to the string with
	 * @return *this
	 *
	 */
	String& String::operator=(char ch)
	{
		assert (ch != '\0');
		
		mLength = 2ul;
		Memory::Memset(&mString[1], '\0', mCapacity);
		mString[0] = ch;

		return *this;
	}

	/**
	 *
  	 * @brief Returns the associated memory pool 
	 * @details Returns the memory pool associated with the string. 
	 * 			@n@n
	 * 			Complexity: constant
	 * @return The associated memory pool
	 *
	 */
	constexpr const MemoryPool& String::GetMemoryPool() const
	{
		return *mPool;
	}


	// Element Access

	/**
	 *
  	 * @brief Accesses the specified character
	 * @details Returns a reference to the character at specified location pos.
	 * 			No bounds checking is performed. If pos > size(), the behavior is undefined.@n
	 * 			If pos == mLength, a reference to the last character (the null character) is returned.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pos position of the character to return
	 * @return Reference to the requested character.
	 *
	 */
	constexpr char& String::operator[](size_t pos)
	{
		if (pos >= mLength)
		{
			pos = mLength;
		}

		return mString[pos];
	}

	/**
	 *
  	 * @brief Accesses the specified character
	 * @details Returns a reference to the character at specified location pos.
	 * 			No bounds checking is performed. If pos > size(), the behavior is undefined.@n
	 * 			If pos == mLength, a reference to the last character (the null character) is returned.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param pos position of the character to return
	 * @return Reference to the requested character.
	 *
	 */
	constexpr const char& String::operator[](size_t pos) const
	{
		if (pos >= mLength)
		{
			pos = mLength;
		}

		return mString[pos];
	}

	/**
	 *
  	 * @brief Accesses the first character 
	 * @details Returns reference to the first character in the string.
	 * 			Assertion fail occurs if <code>IsEmpty() == true<code>
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the first character, equivalent to <code>operator[](0)</code>.
	 *
	 */
	constexpr char& String::GetFront()
	{
		assert(mString != nullptr);

		return mString[0];
	}

	/**
	 *
  	 * @brief Accesses the first character 
	 * @details Returns reference to the first character in the string.
	 * 			Assertion fail occurs if <code>IsEmpty() == true<code>
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the first character, equivalent to <code>operator[](0)</code>.
	 *
	 */
	constexpr const char& String::GetFront() const
	{
		assert(mString != nullptr);

		return mString[0];
	}

	/**
	 *
  	 * @brief Accesses the last character 
	 * @details Returns reference to the last character in the string.
	 * 			Assertion fail occurs if <code>IsEmpty() == true<code>
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the last character, equivalent to <code>operator[](GetLength() - 1)</code>.
	 *
	 */
	constexpr char& String::GetBack()
	{
		assert(mString != nullptr);

		return mString[mLength - 1];
	}

	/**
	 *
  	 * @brief Accesses the last character 
	 * @details Returns reference to the last character in the string.
	 * 			Assertion fail occurs if <code>IsEmpty() == true<code>
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Reference to the last character, equivalent to <code>operator[](GetLength() - 1)</code>.
	 *
	 */
	constexpr const char& String::GetBack() const
	{
		assert(mString != nullptr);

		return mString[mLength - 1];
	}

	/**
	 *
  	 * @brief Returns a non-modifiable standard C character array version of the string
	 * @details Returns a pointer to a null-terminated character array with data equivalent to those stored in the string.@n
	 * 			The pointer is such that the range [GetCString(); GetCString() + GetLength()] is valid and the values in it correspond to the values stored in the string 
	 * 			with an additional null character after the last position.
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Pointer to the underlying character storage.@n
	 * 			<code>GetCString() + i == std::addressof(operator[](i))</code> for every <code>i</code> in <code>[0, GetLength()]</code>.
	 *
	 */
	const char* String::GetCString() const
	{
		return mString;
	}

	/**
	 *
  	 * @brief Checks whether the string is empty 
	 * @details Checks if the string has no characters.
	 * 			@n@n
	 * 			Complexity: constant
	 * @return <code>true</code> if the string is empty, <code>false</code> otherwise 
	 *
	 */
	[[nodiscard]] constexpr bool String::IsEmpty() const noexcept
	{
		return mLength == 0ul;
	}

	/**
	 *
  	 * @brief Returns the number of characters 
	 * @details Returns the number of <code>char</code> elements in the string
	 * 			@n@n
	 * 			Complexity: constant
	 * @return The number of <code>char</code> elements in the string. 
	 *
	 */
	constexpr size_t String::GetLength() const noexcept
	{
		return mLength;
	}

	/**
	 *
  	 * @brief Returns the maximum number of characters
	 * @details Returns the maximum number of elements the string is able to hold due to system or library implementation limitations.
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Maximum number of characters.
	 *
	 */
	constexpr size_t String::GetMaxSize() const noexcept
	{
		return mPool->GetFreeMemorySize();
	}

	/**
	 *
  	 * @brief Reserves storage
	 * @details Informs a cave::String object of a planned change in size, so that it can manage the storage allocation appropriately.@n
	 * 			If newCapacity is greater than the current GetCapacity(), new storage is allocated, and GetCapacity() is made equal or greater than newCapacity.@n
	 * 			If newCapacity is less than or equal to the current GetCapacity(), there is no effect.@n
	 * 			 If a capacity change takes place, all iterators and references, including the past-the-end iterator, are invalidated. 
	 * 			@n@n
	 * 			Complexity: at most linear in the GetSize() of the string
	 * @param newCapacity new capacity of the string
	 *
	 */
	constexpr void String::SetCapacity(size_t newCapacity)
	{
		++newCapacity;
		if (newCapacity > mCapacity)
		{
			char* newString = reinterpret_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', newCapacity - mLength);
			
			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mCapacity = newCapacity;
			mString = newString;
		}
	}

	/**
	 *
  	 * @brief Returns the number of characters that can be held in currently allocated storage 
	 * @details Returns the number of characters that the string has currently allocated space for.
	 * 			@n@n
	 * 			Complexity: constant
	 * @return Capacity of the currently allocated storage, i.e. the storage available for storing elements.
	 *
	 */
	constexpr size_t String::GetCapacity() const noexcept
	{
		return mCapacity - 1ul;
	}

	/**
	 *
  	 * @brief Reduces memory usage by freeing unused memory
	 * @details Requests the removal of unused capacity.@n
	 * 			It is a non-binding request to reduce GetCapacity() to GetSize(). It depends on the implementation if the request is fulfilled.@n
	 * 			If (and only if) reallocation takes place, all pointers, references, and iterators are invalidated.
	 * 			@n@n
	 * 			Complexity: linear in the size of the string
	 *
	 */
	constexpr void String::Shrink()
	{
		size_t fitCapacity = GetSufficientCapacity<ALIGNED_BYTE>(mLength);
		if (mCapacity != fitCapacity)
		{
			char* newString = static_cast<char*>(mPool->Allocate(fitCapacity * sizeof(char)));
			Strcpy(newString, fitCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', fitCapacity - mLength);
			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mCapacity = fitCapacity;
			mString = newString;
		}
	}

	/**
	 *
  	 * @brief Clears the contents 
	 * @details Removes all characters from the string.@n
	 * 			All pointers, references, and iterators are invalidated. 
	 * 			@n@n
	 * 			Complexity: Linear in the size of the string, although existing implementations operate in constant time.
	 * @note Current implementation do not change capacity. This means that they do not release the allocated memory
	 *
	 */
	constexpr void String::Clear() noexcept
	{
		Memory::Memset(mString, '\0', mLength);
		mLength = 0ul;
	}

	/**
	 *
  	 * @brief (1) Inserts characters 
	 * @details Inserts count copies of character ch at the position index
	 * @param index position at which the content will be inserted
	 * @param ch character to insert
	 * @param count number of characters to insert
	 * @return true if insertion succeeds, false otherwise.
	 *
	 */
	constexpr bool String::InsertAt(size_t index, size_t count, char ch)
	{
		if (index > mLength)
		{
			return false;
		}

		if (index + count + 1 >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
#endif
			char* newString = static_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', newCapacity - mLength);

			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = newString;
			mCapacity = newCapacity;
		}

		for (size_t i = mLength + 1; i > index; --i)
		{
			mString[i - 1 + count] = mString[i - 1];
		}

		mLength = mLength + count;

		Memory::Memset(&mString[index], ch, count);

		return true;
	}

	/**
	 *
  	 * @brief (2) Inserts characters 
	 * @details Inserts null-terminated character string pointed to by s at the position index. 
	 * 			The length of the string is determined by the first null character using Strlen(s).
	 * @param index position at which the content will be inserted
	 * @param s pointer to the character string to insert
	 * @return true if insertion succeeds, false otherwise.
	 *
	 */
	constexpr bool String::InsertAt(size_t index, const char* s)
	{
		if (s == nullptr)
		{
			return false;
		}

		return InsertAt(index, s, Strlen(s));
	}

	/**
	 *
  	 * @brief (3) Inserts characters 
	 * @details Inserts the characters in the range [s, s+count) at the position index. 
	 * 			The range can contain null characters.
	 * @param index position at which the content will be inserted
	 * @param count number of characters to insert
	 * @param s pointer to the character string to insert
	 * @return true if insertion succeeds, false otherwise.
	 *
	 */
	constexpr bool String::InsertAt(size_t index, const char* s, size_t count)
	{
		if (index > mLength || s == nullptr)
		{
			return false;
		}

		if (index + count + 1 >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
#endif
			char* newString = static_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', newCapacity - mLength);

			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = newString;
			mCapacity = newCapacity;
		}

		for (size_t i = mLength + 1; i > index; --i)
		{
			mString[i - 1 + count] = mString[i - 1];
		}
		mLength = mLength + count;

		Strcpy(&mString[index], mCapacity - index, s, count);

		return true;
	}

	/**
	 *
  	 * @brief (4) Inserts characters 
	 * @details Inserts string str at the position index
	 * @param index position at which the content will be inserted
	 * @param str string to insert
	 * @return true if insertion succeeds, false otherwise.
	 *
	 */
	constexpr bool String::InsertAt(size_t index, const String& str)
	{
		if (index > mLength)
		{
			return false;
		}

		if (index + str.mLength + 1 >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
#endif
			char* newString = static_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', mCapacity - mLength);

			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = newString;
			mCapacity = newCapacity;
		}

		for (size_t i = mLength + 1; i > index; --i)
		{
			mString[i - 1 + str.mLength] = mString[i - 1];
		}

		mLength = mLength + str.mLength;

		Strcpy(&mString[index], mCapacity - index, str.mString, str.mLength);

		return true;
	}

	/**
	 *
  	 * @brief (5) Inserts characters 
	 * @details Inserts a string, obtained by str.GetSubstring(indexStr, count) at the position index
	 * @param index position at which the content will be inserted
	 * @param count number of characters to insert
	 * @param str string to insert
	 * @param indexStr position of the first character in the string str to insert
	 * @return true if insertion succeeds, false otherwise.
	 *
	 */
	constexpr bool String::InsertAt(size_t index, const String& str, size_t indexStr, size_t count)
	{
		if (index > mLength || indexStr > str.mLength)
		{
			return false;
		}

		if (index + str.mLength + 1 >= mCapacity)
		{
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
#endif
			char* newString = static_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', mCapacity - mLength);

			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = newString;
			mCapacity = newCapacity;
		}

		for (size_t i = mLength + 1; i > index; --i)
		{
			mString[i - 1 + str.mLength] = mString[i - 1];
		}

		mLength = mLength + count;

		Strcpy(&mString[index], mCapacity - index, &str.mString[indexStr], count);

		return true;
	}

	/**
	 *
  	 * @brief Removes characters
	 * @details Removes min(count, GetLength() - index) characters starting at index.
	 * @param index first character to remove
	 * @param count number of characters to remove
	 * @return true if removal succeeds, false otherwise.
	 *
	 */
	constexpr bool String::RemoveAt(size_t index, size_t count)
	{
		if (index + count > mLength)
		{
			return false;
		}

		for (; mString[index] != '\0'; ++index)
		{
			mString[index] = mString[index + count];
		}

		return true;
	}

	/**
	 *
  	 * @brief Appends a character to the end
	 * @details Appends the given character ch to the end of the string.
	 * 			@n@n
	 * 			Complexity: amortized constant
	 * @param ch the character to append
	 *
	 */
	constexpr void String::PushBack(char ch)
	{
		if (ch == '\0')
		{
			return;
		}

		if (mLength + 1 >= mCapacity)
		{
			// Reallocate memory
			size_t newLength = mLength + 1;
#if CAPACITY_INCREASE_MODE == CAPACITY_INCREASE_MODE_DOUBLE
			size_t newCapacity = mCapacity * 2;
#else
			size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
#endif
			char* newString = static_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));

			// Copy s to mString
			Strcpy(newString, newCapacity, mString, mLength);
			Memory::Memset(&newString[mLength], '\0', newCapacity - newLength);
			mPool->Deallocate(mString, mCapacity * sizeof(char));
			mString = newString;
			mLength = newLength;
			mCapacity = newCapacity;
		}

		mString[mLength] = ch;
		++mLength;
	}

	/**
	 *
  	 * @brief Removes the last character
	 * @details Removes the last character from the string.
	 * 			@n@n
	 * 			Complexity: constant
	 *
	 */
	constexpr void String::PopBack()
	{
		if (mLength > 0ul)
		{
			--mLength;
		}
	}


	/**
	 *
  	 * @brief (1) Appends characters to the end
	 * @details Appends count copies of character ch
	 * 			@n@n
	 * 			Complexity: constant
	 * @param count number of characters to append
	 * @param ch character value to append
	 *
	 */
	constexpr void String::Append(size_t count, char ch)
	{
		InsertAt(mLength, count, ch);
	}

	/**
	 *
  	 * @brief (2) Appends characters to the end
	 * @details Appends string str
	 * 			@n@n
	 * 			Complexity: constant
	 * @param str string to append
	 *
	 */
	constexpr void String::Append(const String& str)
	{
		InsertAt(mLength, str);
	}

	/**
	 *
  	 * @brief (3) Appends characters to the end
	 * @details Appends a substring [pos, pos+count) of str. 
	 * 			If the requested substring lasts past the end of the string, or if count == NPOS, the appended substring is [pos, GetLength()). 
	 * 			@n@n
	 * 			Complexity: constant
	 * @param count number of characters to append
	 * @param pos the index of the first character to append
	 * @param str string to append
	 *
	 */
	constexpr void String::Append(const String& str, size_t pos, size_t count)
	{
		InsertAt(mLength, str, pos, count);
	}

	/**
	 *
  	 * @brief (4) Appends characters to the end
	 * @details Appends characters in the range <code>[s, s + count)</code>. 
	 * 			This range can contain null characters.
	 * 			@n@n
	 * 			Complexity: constant
	 * @param count number of characters to append
	 * @param s pointer to the character string to append
	 *
	 */
	constexpr void String::Append(const char* s, size_t count)
	{
		InsertAt(mLength, s, count);
	}

	/**
	 *
  	 * @brief (5) Appends characters to the end
	 * @details Appends the null-terminated character string pointed to by s. 
	 * 			The length of the string is determined by the first null character using <code>Strlen(s)</code>. 
	 * 			@n@n
	 * 			Complexity: constant
	 * @param s pointer to the character string to append
	 *
	 */
	constexpr void String::Append(const char* s)
	{
		InsertAt(mLength, s);
	}

	/**
	 *
  	 * @brief (1) Appends characters to the end
	 * @details Appends string str
	 * 			@n@n
	 * 			Complexity: linear in str.GetLength()
	 * @param str string to append
	 * @return *this
	 *
	 */
	String& String::operator+=(const String& str)
	{
		Append(str.mString);

		return *this;
	}

	/**
	 *
  	 * @brief (2) Appends characters to the end
	 * @details Appends character ch
	 * 			@n@n
	 * 			Complexity: constant
	 * @param ch character value to append
	 * @return *this
	 *
	 */
	String& String::operator+=(char ch)
	{
		Append(1ul, ch);

		return *this;
	}

	/**
	 *
  	 * @brief (3) Appends characters to the end
	 * @details Appends the null-terminated character string pointed to by s. 
	 * 			@n@n
	 * 			Complexity: linear in size of s
	 * @param s pointer to a null-terminated character string to append 
	 * @return *this
	 *
	 */
	String& String::operator+=(const char* s)
	{
		Append(s);

		return *this;
	}

	/**
	 *
  	 * @brief (1) Checks if the string starts with the given prefix
	 * @details Checks if the string begins with the given prefix, a string str.
	 * @param str a string
	 * @return true if the string begins with the provided prefix, false otherwise. 
	 *
	 */
	constexpr bool String::StartsWith(const String& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < str.mLength; ++i)
		{
			if (mString[i] != str.mString[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (1) Checks if the string starts with the given prefix
	 * @details Checks if the string begins with the given prefix, a string str.
	 * @param str a string
	 * @return true if the string begins with the provided prefix, false otherwise. 
	 *
	 */
	constexpr bool String::StartsWith(const String&& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < str.mLength; ++i)
		{
			if (mString[i] != str.mString[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (2) Checks if the string starts with the given prefix
	 * @details Checks if the string begins with the given prefix, a single character c.
	 * @param c a single character
	 * @return true if the string begins with the provided prefix, false otherwise. 
	 *
	 */
	constexpr bool String::StartsWith(char c) const noexcept
	{
		if (mLength <= 0ul)
		{
			return false;
		}

		return mString[0] == c;
	}

	/**
	 *
  	 * @brief (3) Checks if the string starts with the given prefix
	 * @details Checks if the string begins with the given prefix, a null-terminated character string s.
	 * @param s a null-terminated character string
	 * @return true if the string begins with the provided prefix, false otherwise. 
	 *
	 */
	constexpr bool String::StartsWith(const char* s) const
	{
		size_t sLength = Strlen(s);
		if (mLength < sLength)
		{
			return false;
		}

		for (size_t i = 0; i < sLength; ++i)
		{
			if (mString[i] != s[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (1) Checks if the string ends with the given suffix 
	 * @details Checks if the string ends with the given suffix, a string str.
	 * @param str a string
	 * @return true if the string ends with the provided suffix, false otherwise.
	 *
	 */
	constexpr bool String::EndsWith(const String& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < str.mLength; ++i)
		{
			if (mString[i + mLength - str.mLength] != str.mString[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (1) Checks if the string ends with the given suffix 
	 * @details Checks if the string ends with the given suffix, a string str.
	 * @param str a string
	 * @return true if the string ends with the provided suffix, false otherwise.
	 *
	 */
	constexpr bool String::EndsWith(const String&& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < str.mLength; ++i)
		{
			if (mString[i + mLength - str.mLength] != str.mString[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (2) Checks if the string starts with the given prefix
	 * @details Checks if the string ends with the given suffix, a single character c.
	 * @param c a single character
	 * @return true if the string ends with the provided suffix, false otherwise.
	 *
	 */
	constexpr bool String::EndsWith(char c) const noexcept
	{
		if (mLength <= 0ul)
		{
			return false;
		}

		return mString[mLength - 1] == c;
	}

	/**
	 *
  	 * @brief (3) Checks if the string starts with the given prefix
	 * @details Checks if the string ends with the given suffix, a null-terminated character string s.
	 * @param s a null-terminated character string
	 * @return true if the string ends with the provided suffix, false otherwise.
	 *
	 */
	constexpr bool String::EndsWith(const char* s) const
	{
		size_t sLength = Strlen(s);
		if (mLength < sLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < sLength; ++i)
		{
			if (mString[i + mLength - sLength] != s[i])
			{
				return false;
			}
		}

		return true;
	}

	/**
	 *
  	 * @brief (1) Checks if the string contains the given substring or character
	 * @details Checks if the string contains the given substring, a string str.
	 * @param str a string
	 * @return true if the string contains the provided substring, false otherwise.
	 *
	 */
	constexpr bool String::Contains(const String& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < mLength; ++i)
		{
			if (mString[i] == str.mString[0])
			{
				if (mLength - i < str.mLength)
				{
					return false;
				}

				size_t strIndex = 1ul;
				for (; strIndex < str.mLength; ++strIndex)
				{
					if (mString[strIndex + i] != str.mString[strIndex])
					{
						break;
					}
				}

				if (strIndex == str.mLength)
				{
					return true;
				}
			}
		}

		return false;
	}

	/**
	 *
  	 * @brief (1) Checks if the string contains the given substring or character
	 * @details Checks if the string contains the given substring, a string str.
	 * @param str a string
	 * @return true if the string contains the provided substring, false otherwise.
	 *
	 */
	constexpr bool String::Contains(const String&& str) const noexcept
	{
		if (mLength < str.mLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < mLength; ++i)
		{
			if (mString[i] == str.mString[0])
			{
				if (mLength - i < str.mLength)
				{
					return false;
				}

				size_t strIndex = 1ul;
				for (; strIndex < str.mLength; ++strIndex)
				{
					if (mString[strIndex + i] != str.mString[strIndex])
					{
						break;
					}
				}

				if (strIndex == str.mLength)
				{
					return true;
				}
			}
		}

		return false;
	}

	/**
	 *
  	 * @brief (2) Checks if the string contains the given substring or character
	 * @details Checks if the string contains the given substring, a string str.
	 * @param c a single character
	 * @return true if the string contains the provided substring, false otherwise.
	 *
	 */
	constexpr bool String::Contains(char c) const noexcept
	{
		if (mLength < 1ul)
		{
			return false;
		}

		for (size_t i = 0ul; i < mLength; ++i)
		{
			if (mString[i] == c)
			{
				return true;
			}
		}

		return false;
	}

	/**
	 *
  	 * @brief (3) Checks if the string contains the given substring or character
	 * @details Checks if the string contains the given substring, a string str.
	 * @param s a null-terminated character string 
	 * @return true if the string contains the provided substring, false otherwise.
	 *
	 */
	constexpr bool String::Contains(const char* s) const
	{
		size_t sLength = Strlen(s);
		if (mLength < sLength)
		{
			return false;
		}

		for (size_t i = 0ul; i < mLength; ++i)
		{
			if (mString[i] == s[0])
			{
				if (mLength - i < sLength)
				{
					return false;
				}

				size_t strIndex = 1ul;
				for (; strIndex < sLength; ++strIndex)
				{
					if (mString[strIndex + i] != s[strIndex])
					{
						break;
					}
				}

				if (strIndex == sLength)
				{
					return true;
				}
			}
		}

		return false;
	}

	/**
	 *
  	 * @brief (1) Replaces specified portion of a string
	 * @details Replaces the part of the string indicated by [pos, pos + count) with a new string, 
	 * 			string str.
	 * @param pos start of the substring that is going to be replaced
	 * @param count length of the substring that is going to be replaced
	 * @param str string to use for replacement
	 * @return *this
	 *
	 */
	constexpr String& String::Replace(size_t pos, size_t count, const String& str)
	{
		return Replace(pos, count, str, 0ul, str.mLength);
	}

	/**
	 *
  	 * @brief (2) Replaces specified portion of a string
	 * @details Replaces the part of the string indicated by [pos, pos + count) with a new string, 
	 * 			substring [pos2, pos2 + count2) of str, except if count2 == NPOS or 
	 * 			if would extend past str.GetLength(), [pos2, str.GetLength()) is used..
	 * @param pos start of the substring that is going to be replaced
	 * @param count length of the substring that is going to be replaced
	 * @param str string to use for replacement
	 * @param pos2 start of the substring to replace with
	 * @param count2 number of characters to replace with
	 * @return *this
	 *
	 */
	constexpr String& String::Replace(size_t pos, size_t count, const String& str, size_t pos2, size_t count2)
	{
		assert(pos <= mLength && pos2 <= str.mLength);

		if (count2 > str.mLength || count2 == NPOS)
		{
			count2 = str.mLength;
		}

		if (count < count2)
		{
			for (size_t i = mLength + 1; i > pos + count; --i)
			{
				mString[i - 1 + count2 - count] = mString[i - 1];
			}
		}
		else
		{
			for (size_t i = pos + count; i < mLength + 1; ++i)
			{
				mString[i - count + count2] = mString[i];
			}
		}

		Strcpy(&mString[pos], mCapacity - pos, &str.mString[pos2], count2);
		if (pos + count >= mLength)
		{
			count = mLength - pos;
		}

		if (pos2 + count2 >= str.mLength)
		{
			count2 = str.mLength - pos2;
		}
		
		mLength = mLength + count2 - count;
		mString[mLength] = '\0';

		return *this;
	}

	/**
	 *
  	 * @brief (3) Replaces specified portion of a string
	 * @details Replaces the part of the string indicated by [pos, pos + count) with a new string, 
	 * 			characters in the range [cstr, cstr + count2);.
	 * @param pos start of the substring that is going to be replaced
	 * @param count length of the substring that is going to be replaced
	 * @param count2 number of characters to replace with
	 * @param cStr pointer to the character string to use for replacement
	 * @return *this
	 *
	 */
	constexpr String& String::Replace(size_t pos, size_t count, const char* cStr, size_t count2)
	{
		assert(cStr != nullptr && pos <= mLength);

		size_t sLength = Strlen(cStr);

		if (count2 > sLength)
		{
			count2 = sLength;
		}

		if (count < count2)
		{
			for (size_t i = mLength + 1; i > pos + count; --i)
			{
				mString[i - 1 + count2 - count] = mString[i - 1];
			}
		}
		else
		{
			for (size_t i = pos + count; i < mLength + 1; ++i)
			{
				mString[i - count + count2] = mString[i];
			}
		}

		Strcpy(&mString[pos], mCapacity - pos, cStr, count2);
		if (pos + count >= mLength)
		{
			count = mLength - pos;
		}
		
		mLength = mLength + count2 - count;
		mString[mLength] = '\0';

		return *this;
	}
	
	/**
	 *
  	 * @brief (4) Replaces specified portion of a string
	 * @details Replaces the part of the string indicated by [pos, pos + count) with a new string, 
	 * 			characters in the range [cStr, cStr + Strlen(cStr));.
	 * @param pos start of the substring that is going to be replaced
	 * @param count length of the substring that is going to be replaced
	 * @param cStr pointer to the character string to use for replacement
	 * @return *this
	 *
	 */
	constexpr String& String::Replace(size_t pos, size_t count, const char* cStr)
	{
		assert(cStr != nullptr);

		return Replace(pos, count, cStr, Strlen(cStr));
	}

	/**
	 *
  	 * @brief (5) Replaces specified portion of a string
	 * @details Replaces the part of the string indicated by [pos, pos + count) with a new string, 
	 * 			count2 copies of character ch;.
	 * @param pos start of the substring that is going to be replaced
	 * @param count length of the substring that is going to be replaced
	 * @param count2 number of characters to replace with
	 * @param ch character value to use for replacement 
	 * @return *this
	 *
	 */
	constexpr String& String::Replace(size_t pos, size_t count, size_t count2, char ch)
	{
		assert(pos <= mLength);

		if (count < count2)
		{
			for (size_t i = mLength + 1; i > pos + count; --i)
			{
				mString[i - 1 + count2 - count] = mString[i - 1];
			}
		}
		else
		{
			for (size_t i = pos + count; i < mLength + 1; ++i)
			{
				mString[i - count + count2] = mString[i];
			}
		}

		Memory::Memset(&mString[pos], ch, count2);
		if (pos + count >= mLength)
		{
			count = mLength - pos;
		}
		
		mLength = mLength + count2 - count;
		mString[mLength] = '\0';

		return *this;
	}

	/**
	 *
  	 * @brief Returns a substring
	 * @details Returns a substring [pos, pos+count). 
	 * 			If the requested substring extends past the end of the string, 
	 * 			or if count == NPOS, the returned substring is [pos, GetLength()). 
	 * @param pos position of the first character to include
	 * @param count length of the substring
	 * @return String containing the substring [pos, pos+count).
	 *
	 */
	String String::GetSubstring(size_t pos, size_t count) const
	{
		assert(pos <= mLength);

		if (count == NPOS || pos + count >= mLength)
		{
			count = mLength - pos;
		}

		return String{*this, pos, count, *mPool};
	}

	/**
	 *
  	 * @brief Changes the number of characters stored
	 * @details Resizes the string to contain count characters.
	 * 			If the current size is less than count, additional characters are appended.
	 * 			If the current size is greater than count, the string is reduced to its first count elements.
	 * 			Initializes new characters to '\0'., the second version initializes new characters to ch. 
	 * @param count new size of the string
	 * @param ch character to initialize the new characters with
	 *
	 */
	constexpr void String::Resize(size_t count)
	{
		Resize(count, '\0');
	}

	/**
	 *
  	 * @brief Changes the number of characters stored
	 * @details Resizes the string to contain count characters.
	 * 			If the current size is less than count, additional characters are appended.
	 * 			If the current size is greater than count, the string is reduced to its first count elements.
	 * 			Initializes new characters to ch. 
	 * @param count new size of the string
	 * @param ch character to initialize the new characters with
	 *
	 */
	constexpr void String::Resize(size_t count, char ch)
	{
		size_t newLength = count;
		if (newLength < mLength)
		{
			Memory::Memset(&mString[newLength], ch, mLength - newLength);
		}
		else
		{
			if (newLength + 1 > mCapacity)
			{
				size_t newCapacity = GetSufficientCapacity<ALIGNED_BYTE>(newLength);
				char* newString = reinterpret_cast<char*>(mPool->Allocate(newCapacity * sizeof(char)));
				Strcpy(newString, newCapacity, mString, mLength);
				Memory::Memset(&newString[mLength], '\0', newCapacity - mLength);
				mPool->Deallocate(mString, mCapacity * sizeof(char));
				mString = newString;
				mCapacity = newCapacity;
			}

			Memory::Memset(&mString[mLength], ch, newLength - mLength);
		}

		mLength = newLength;
		mString[mLength] = '\0';
	}

	/**
	 *
  	 * @brief (1) Find characters in the string
	 * @details Finds the first substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position preceding pos.
	 * @param str string to search for
	 * @param pos position at which to start the search
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 *
	 */
	constexpr size_t String::GetIndexOf(const String& str, size_t pos) const noexcept
	{
		return GetIndexOf(str.mString, pos);
	}

	/**
	 *
  	 * @brief (2) Find characters in the string
	 * @details Finds the first substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position preceding pos.
	 * @param pos position at which to start the search
	 * @param count length of substring to search for
	 * @param s pointer to a character string to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 *
	 */
	constexpr size_t String::GetIndexOf(const char* s, size_t pos, size_t count) const
	{
		if (s == nullptr)
		{
			return NPOS;
		}

		char* temp = &mString[pos];
		size_t index = pos;

		while (index - pos < count)
		{
			size_t counter = index - pos;
			const char* checkS = s;
			// Find first index of mString that either is NULL or is same to first character of s
			for (; counter < count && *temp != '\0' && *temp != *checkS; ++temp, ++index, ++counter)
			{
			}

			// Fail. No identical first letter character
			if (*temp == '\0')
			{
				return NPOS;
			}

			// Check whether s and rest of characters of mString resembles
			for (; counter < count && *temp != '\0' && *temp == *checkS; ++temp, ++checkS, ++counter)
			{
			}

			// Success. Resembles
			if (*checkS == '\0')
			{
				return index;
			}

			++index;
		}

		return NPOS;
	}

	/**
	 *
  	 * @brief (3) Find characters in the string
	 * @details Finds the first substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position preceding pos.
	 * @param pos position at which to start the search
	 * @param s pointer to a character string to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 *
	 */
	constexpr size_t String::GetIndexOf(const char* s, size_t pos) const
	{
		assert(s != nullptr);

		return GetIndexOf(s, pos, Strlen(s));
	}

	/**
	 *
  	 * @brief (4) Find characters in the string
	 * @details Finds the first substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position preceding pos.
	 * @param pos position at which to start the search
	 * @param ch character to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 *
	 */
	constexpr size_t String::GetIndexOf(char ch, size_t pos) const
	{
		for (size_t i = pos; i < mLength; ++i)
		{
			if (mString[i] == ch)
			{
				return i;
			}
		}

		return NPOS;
	}

	/**
	 *
  	 * @brief (1) Find the last occurrence of a substring
	 * @details Finds the last substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position following pos. 
	 * 			If NPOS or any value not smaller than GetLength() - 1 is passed as pos, whole string will be searched.@n 
	 * 			Finds the last substring equal to str.
	 * @param str string to search for
	 * @param pos position at which to begin searching
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 * 			Note that this is an offset from the start of the string, not the end.@n
	 * 			If searching for an empty string (str.size(), count, or Strlen(s) is zero) returns pos (the empty string is found immediately) 
	 * 			unless pos > GetLength() (including the case where pos == NPOS), in which case returns GetLength().@n
	 * 			Otherwise, if GetLength() is zero, NPOS is always returned. 
	 *
	 */
	constexpr size_t String::GetLastIndexOf(const String& str, size_t pos) const noexcept
	{
		return GetLastIndexOf(str.mString, pos, str.mLength);
	}

	/**
	 *
  	 * @brief (2) Find the last occurrence of a substring
	 * @details Finds the last substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position following pos. 
	 * 			If NPOS or any value not smaller than GetLength() - 1 is passed as pos, whole string will be searched.@n
	 * 			Finds the last substring equal to the range [s, s+count). This range can include null characters.  
	 * @param pos position at which to begin searching
	 * @param count length of substring to search for
	 * @param s pointer to a character string to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 * 			Note that this is an offset from the start of the string, not the end.@n
	 * 			If searching for an empty string (str.size(), count, or Strlen(s) is zero) returns pos (the empty string is found immediately) 
	 * 			unless pos > GetLength() (including the case where pos == NPOS), in which case returns GetLength().@n
	 * 			Otherwise, if GetLength() is zero, NPOS is always returned. 
	 *
	 */
	constexpr size_t String::GetLastIndexOf(const char* s, size_t pos, size_t count) const
	{
		if (s == nullptr)
		{
			return NPOS;
		}
		
		if (*s == '\0')
		{
			if (mLength > 0)
			{
				return mLength;
			}

			return 0;
		}

		if (*mString == '\0')
		{
			return NPOS;
		}

		if (pos == NPOS || pos >= mLength  - 1)
		{
			pos = mLength - 1;
		}

		char* temp = mString + pos;
		size_t index = pos;

		while (index - pos < count)
		{
			const char* checkS = s;
			size_t counter = index - pos;
			// Find last index of mString that either is first character or is same to first character of s
			for (; counter < count && temp != (mString - 1) && *temp != *checkS; --temp, --index, ++counter)
			{
			}

			// Fail. No identical character
			if (temp == (mString - 1))
			{
				return NPOS;
			}

			char* firstCharacter = temp;
			// Check whether s and rest of characters of mString resembles
			for (; counter < count && *firstCharacter != '\0' && *firstCharacter == *checkS; ++firstCharacter, ++checkS, ++counter)
			{
			}

			// Success. Resembles
			if (*checkS == '\0')
			{
				return index;
			}

			--temp;
			--index;
		}

		return NPOS;
	}

	/**
	 *
  	 * @brief (3) Find the last occurrence of a substring
	 * @details Finds the last substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position following pos. 
	 * 			If NPOS or any value not smaller than GetLength() - 1 is passed as pos, whole string will be searched.@n 
	 * 			Finds the last substring equal to the character string pointed to by s. 
	 * 			The length of the string is determined by the first null character using Strlen(s).
	 * @param pos position at which to begin searching
	 * @param s pointer to a character string to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 * 			Note that this is an offset from the start of the string, not the end.@n
	 * 			If searching for an empty string (str.size(), count, or Strlen(s) is zero) returns pos (the empty string is found immediately) 
	 * 			unless pos > GetLength() (including the case where pos == NPOS), in which case returns GetLength().@n
	 * 			Otherwise, if GetLength() is zero, NPOS is always returned. 
	 *
	 */
	constexpr size_t String::GetLastIndexOf(const char* s, size_t pos) const
	{
		assert(s != nullptr);

		return GetLastIndexOf(s, pos, Strlen(s));
	}

	/**
	 *
  	 * @brief (4) Find the last occurrence of a substring
	 * @details Finds the last substring equal to the given character sequence. 
	 * 			Search begins at pos, i.e. the found substring must not begin in a position following pos. 
	 * 			If NPOS or any value not smaller than GetLength() - 1 is passed as pos, whole string will be searched.@n 
	 * 			Finds the last character equal to ch.
	 * @param pos position at which to begin searching
	 * @param ch character to search for
	 * @return Position of the first character of the found substring or npos if no such substring is found. 
	 * 			Note that this is an offset from the start of the string, not the end.@n
	 * 			If searching for an empty string (str.size(), count, or Strlen(s) is zero) returns pos (the empty string is found immediately) 
	 * 			unless pos > GetLength() (including the case where pos == NPOS), in which case returns GetLength().@n
	 * 			Otherwise, if GetLength() is zero, NPOS is always returned. 
	 *
	 */
	constexpr size_t String::GetLastIndexOf(char ch, size_t pos) const
	{
		for (size_t i = mLength; i > pos; --i)
		{
			if (mString[i - 1] == ch)
			{
				return i - 1;
			}
		}

		return NPOS;
	}

	/**
	 *
  	 * @brief (1) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const String& lhs, const String& rhs)
	{
		String s(lhs);

		// Append other to s
		s.Append(rhs.mString);

		return s;
	}

	/**
	 *
  	 * @brief (2) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const String& lhs, const char* rhs)
	{
		String s(lhs);
		s.Append(rhs);
		return s;
	}

	/**
	 *
  	 * @brief (3) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const String& lhs, char rhs)
	{
		String s(lhs);
		s.Append(1, rhs);
		return s;
	}

	/**
	 *
  	 * @brief (4) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is rhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const char* lhs, const String& rhs)
	{
		String s(lhs, *rhs.mPool);
		s.Append(rhs.mString);
		return s;
	}

	/**
	 *
  	 * @brief (5) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is rhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(char lhs, const String& rhs)
	{
		String s(1, lhs, *rhs.mPool);
		s.Append(rhs.mString);
		return s;
	}

	/**
	 *
  	 * @brief (6) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(String&& lhs, String&& rhs)
	{
		String s(std::move(lhs));
		s.Append(rhs.mString);
		rhs.~String();
		return s;
	}

	/**
	 *
  	 * @brief (7) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(String&& lhs, const String& rhs)
	{
		String s(std::move(lhs));
		s.Append(rhs.mString);
		return s;
	}

	/**
	 *
  	 * @brief (8) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(String&& lhs, const char* rhs)
	{
		String s(std::move(lhs));
		s.Append(rhs);
		return s;
	}

	/**
	 *
  	 * @brief (9) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is lhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(String&& lhs, char rhs)
	{
		String s(std::move(lhs));
		s.Append(1, rhs);
		return s;
	}
	
	/**
	 *
  	 * @brief (10) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is rhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const String& lhs, String&& rhs)
	{
		String s(lhs, *rhs.mPool);
		s.Append(rhs.mString);
		rhs.~String();
		return s;
	}

	/**
	 *
  	 * @brief (11) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is rhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(const char* lhs, String&& rhs)
	{
		String s(lhs, *rhs.mPool);
		s.Append(rhs.mString);
		rhs.~String();
		return s;
	}

	/**
	 *
  	 * @brief (12) Concatenates two strings or a string and a char
	 * @details Returns a string containing characters from lhs followed by the characters from rhs.@n
	 * 			The allocator used for the result is rhs.GetMemoryPool()
	 * @param lhs string, character, or pointer to the first character in a null-terminated array
	 * @param rhs string, character, or pointer to the first character in a null-terminated array 
	 * @return A string containing characters from lhs followed by the characters from rhs, using the memory pool determined as above.
	 *
	 */
	String operator+(char lhs, String&& rhs)
	{
		String s(1, lhs, *rhs.mPool);
		s.Append(rhs.mString);
		rhs.~String();
		return s;
	}

	/**
	 *
  	 * @brief (1) Lexicographically compares two strings
	 * @details Compares the contents of a string with another string or a null-terminated array of CharT.@n 
	 * 			Two strings are equal if both the size of lhs and rhs are equal and 
	 * 			each character in lhs has equivalent character in rhs at the same position.@n
	 * 			Compares two String objects.
	 * 			@n@n 
	 * 			Complexity: linear in the size of the strings.
	 * @param lhs string whose contents to compare
	 * @param rhs string whose contents to compare
	 * @return true if the corresponding comparison holds, false otherwise.
	 *
	 */
	constexpr bool operator==(const String& lhs, const String& rhs) noexcept
	{

		if (lhs.mLength != rhs.mLength)
		{
			return false;
		}

		const char* lhsString = lhs.mString;
		const char* rhsString = rhs.mString;
		if (rhsString != nullptr)
		{
			for (; *lhsString == *rhsString && *lhsString != '\0' && *rhsString != '\0'; ++lhsString, ++rhsString)
			{
			}

			if (*lhsString == '\0' && *rhsString == '\0')
			{
				return true;
			}
		}

		return false;
	}

	/**
	 *
  	 * @brief (2) Lexicographically compares two strings
	 * @details Compares the contents of a string with another string or a null-terminated array of CharT.@n 
	 * 			Two strings are equal if both the size of lhs and rhs are equal and 
	 * 			each character in lhs has equivalent character in rhs at the same position.@n
	 * 			Compares two String objects.
	 * 			@n@n 
	 * 			Complexity: linear in the size of the strings.
	 * @param lhs string whose contents to compare
	 * @param rhs string whose contents to compare
	 * @return true if the corresponding comparison holds, false otherwise.
	 *
	 */
	constexpr bool operator!=(const String& lhs, const String& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	/**
	 *
  	 * @brief (3) Lexicographically compares two strings
	 * @details Compares the contents of a string with another string or a null-terminated array of CharT.@n 
	 * 			Two strings are equal if both the size of lhs and rhs are equal and 
	 * 			each character in lhs has equivalent character in rhs at the same position.@n
	 * 			Compares a String object and a null-terminated array of char.
	 * 			@n@n 
	 * 			Complexity: linear in the size of the strings.
	 * @param lhs string whose contents to compare
	 * @param rhs string whose contents to compare
	 * @return true if the corresponding comparison holds, false otherwise.
	 *
	 */
	constexpr bool operator==(const String& lhs, const char* rhs) noexcept
	{
		size_t rhsLength = Strlen(rhs);

		if (lhs.mLength != rhsLength)
		{
			return false;
		}

		const char* lhsString = lhs.mString;
		const char* rhsString = rhs;
		if (rhsString != nullptr)
		{
			for (; *lhsString == *rhsString && *lhsString != '\0' && *rhsString != '\0'; ++lhsString, ++rhsString)
			{
			}

			if (*lhsString == '\0' && *rhsString == '\0')
			{
				return true;
			}
		}
		
		return false;
	}

	/**
	 *
  	 * @brief (4) Lexicographically compares two strings
	 * @details Compares the contents of a string with another string or a null-terminated array of CharT.@n 
	 * 			Two strings are equal if both the size of lhs and rhs are equal and 
	 * 			each character in lhs has equivalent character in rhs at the same position.@n
	 * 			Compares a String object and a null-terminated array of char.
	 * 			@n@n 
	 * 			Complexity: linear in the size of the strings.
	 * @param lhs string whose contents to compare
	 * @param rhs string whose contents to compare
	 * @return true if the corresponding comparison holds, false otherwise.
	 *
	 */
	constexpr bool operator!=(const String& lhs, const char* rhs) noexcept
	{
		return !(lhs == rhs);
	}

	/**
	 *
  	 * @brief Performs stream output on strings
	 * @details Behaves as a FormattedOutputFunction. After constructing and checking the sentry object, determines the output format padding as follows:@n
	 * 			a) If str.GetLength() is not less than os.width(), uses the range [&str[0], &str[str.GetLength()]) as-is @n
	 * 			b) Otherwise, if (os.flags() & ios_base::adjustfield) == ios_base::left, @n
	 * 			os.width()-str.GetLength() copies of the os.fill() character after the character sequence@n
	 * 			c) Otherwise, places os.width()-str.GetLength() copies of the os.fill() character before the character sequence@n
	 * 			Then stores each character from the resulting sequence (the contents of str plus padding) to the output stream os 
	 * 			as if by calling os.rdbuf()->sputn(seq, n), where n=std::max(os.width(), str.GetLength())@n
	 * 			Finally, calls os.width(0) to cancel the effects of std::setw, if any.
	 * @param os a character output stream
	 * @param str the string to be inserted or extracted 
	 * @return os
	 *
	 */
	std::ostream& operator<<(std::ostream& os, const String& str)
	{
		if (str.GetLength() >= static_cast<size_t>(os.width()))
		{
			os.rdbuf()->sputn(str.mString, str.GetLength());
		}
		else if (os.flags() & (std::ios_base::adjustfield == std::ios_base::left))
		{
			os.rdbuf()->sputn(str.mString, str.GetLength());
			for (size_t i = 0ul; i < os.width() - str.GetLength(); ++i)
			{
				os.rdbuf()->sputc(os.fill());
			}
		}
		else
		{
			for (size_t i = 0ul; i < os.width() - str.GetLength(); ++i)
			{
				os.rdbuf()->sputc(os.fill());
			}
			os.rdbuf()->sputn(str.mString, str.GetLength());
		}

		os.width(0);

		return os;
	}

	/**
	 *
  	 * @brief Performs stream input on strings
	 * @details Behaves as a FormattedInputFunction. After constructing and checking the sentry object, which may skip leading whitespace, 
	 * 			first clears str with str.Clear(), then reads characters from is and appends them to str as if by str.Append(1, c), 
	 * 			until one of the following conditions becomes true:@n
	 * 			* N characters are read, where N is is.width() if is.width() > 0, otherwise N is str.GetCapacity()@n
	 * 			* the end-of-file condition occurs in the stream is@n
	 * 			* std::isspace(c,is.getloc()) is true for the next character c in is (this whitespace character remains in the input stream).@n
	 * 			If no characters are extracted then std::ios::failbit is set on is, which may throw std::ios_base::failure.@n
	 * 			Finally, calls is.width(0) to cancel the effects of std::setw, if any. 
	 * @param is a character input stream
	 * @param str the string to be inserted or extracted 
	 * @return is
	 *
	 */
	std::istream& operator>>(std::istream& is, String& str)
	{
		str.Clear();

		size_t readCharacterCount = 0ul;
		// bool isEof = false;
		size_t width = (is.width() > 0) ? is.width() : str.GetMaxSize();
		for
		(
			char c = static_cast<char>(is.get()); 
			readCharacterCount < width && !is.eof() && !std::isspace(c, is.getloc()); 
			++readCharacterCount, c = static_cast<char>(is.get())
		)
		{
			str.Append(1, c);
			// isEof = is.eof();
		}

		if (str.GetLength() <= 0)
		{
			try
			{
				is.setstate(std::ios::failbit);
			} catch (const std::ios_base::failure& e)
			{
				LOGEF(eLogChannel::CORE_STRING, std::cerr, "%s", e.what());
			}
		}

		is.width(0);

		return is;
	}

	/**
	 *
  	 * @brief (1) Read data from an I/O stream into a string
	 * @details GetLine reads characters from an input stream and places them into a string:@n
	 * 			Behaves as UnformattedInputFunction, except that input.gcount() is not affected. 
	 * 			After constructing and checking the sentry object, performs the following:@n
	 * 			1) Calls str.Clear()@n
	 * 			2) Extracts characters from input and appends them to str until one of the following occurs (checked in the order listed)@n
	 * 				a) end-of-file condition on input, in which case, GetLine sets eofbit.@n
	 * 				b) the next available input character is delim, as tested by c == delim, 
	 * 				in which case the delimiter character is extracted from input, but is not appended to str.@n
	 * 				c) str.GetMaxSize() characters have been stored, in which case getline sets failbit and returns.@n
	 * 			3) If no characters were extracted for whatever reason (not even the discarded delimiter), GetLine sets failbit and returns.
	 * @param input the stream to get data from
	 * @param str the string to put the data into
	 * @param delim the delimiter character
	 * @return input
	 *
	 */
	std::istream& GetLine(std::istream& input, String& str, char delim)
	{
		str.Clear();

		size_t readCharacterCount = 0ul;
		size_t maxSize = str.GetMaxSize();
		bool isEof = false;
		// size_t width = (input.width() > 0) ? input.width() : str.GetMaxSize();
		for
		(
			char c = static_cast<char>(input.get()); 
			!input.eof() && c != delim && readCharacterCount < maxSize; 
			++readCharacterCount, c = static_cast<char>(input.get())
		)
		{
			str.Append(1, c);
			isEof = input.eof();
		}

		if (isEof)
		{
			try
			{
				input.setstate(std::ios::eofbit);
			}
			catch (const std::ios_base::failure& e)
			{
				LOGEF(eLogChannel::CORE_STRING, std::cerr, "%s", e.what());
			}
		}

		if (str.GetLength() <= 0 || readCharacterCount >= maxSize)
		{
			try
			{
				input.setstate(std::ios::failbit);
			}
			catch (const std::ios_base::failure& e)
			{
				LOGEF(eLogChannel::CORE_STRING, std::cerr, "%s", e.what());
			}
		}

		return input;
	}

	/**
	 *
  	 * @brief (1) Read data from an I/O stream into a string
	 * @details GetLine reads characters from an input stream and places them into a string:@n
	 * 			Behaves as UnformattedInputFunction, except that input.gcount() is not affected. 
	 * 			After constructing and checking the sentry object, performs the following:@n
	 * 			1) Calls str.Clear()@n
	 * 			2) Extracts characters from input and appends them to str until one of the following occurs (checked in the order listed)@n
	 * 				a) end-of-file condition on input, in which case, GetLine sets eofbit.@n
	 * 				b) the next available input character is delim, as tested by c == delim, 
	 * 				in which case the delimiter character is extracted from input, but is not appended to str.@n
	 * 				c) str.GetMaxSize() characters have been stored, in which case getline sets failbit and returns.@n
	 * 			3) If no characters were extracted for whatever reason (not even the discarded delimiter), GetLine sets failbit and returns.
	 * @param input the stream to get data from
	 * @param str the string to put the data into
	 * @param delim the delimiter character
	 * @return input
	 *
	 */
	std::istream& GetLine(std::istream&& input, String& str, char delim)
	{
		str.Clear();

		size_t readCharacterCount = 0ul;
		size_t maxSize = str.GetMaxSize();
		bool isEof = false;
		// size_t width = (input.width() > 0) ? input.width() : str.GetMaxSize();
		for (
			char c = static_cast<char>(input.get()); 
			!input.eof() && c != delim && readCharacterCount < maxSize; 
			++readCharacterCount, c = static_cast<char>(input.get())
		)
		{
			str.Append(1, c);
			isEof = input.eof();
		}

		if (isEof)
		{
			try
			{
				input.setstate(std::ios::eofbit);
			}
			catch (const std::ios_base::failure& e)
			{
				LOGEF(eLogChannel::CORE_STRING, std::cerr, "%s", e.what());
			}
		}

		if (str.GetLength() <= 0 || readCharacterCount >= maxSize)
		{
			try
			{
				input.setstate(std::ios::failbit);
			}
			catch (const std::ios_base::failure& e)
			{
				LOGEF(eLogChannel::CORE_STRING, std::cerr, "%s", e.what());
			}
		}

		return input;
	}

	/**
	 *
  	 * @brief (2) Read data from an I/O stream into a string
	 * @details GetLine reads characters from an input stream and places them into a string:@n
	 * 			Same as GetLine(input, str, input.widen('\n')), that is, the default delimiter is the endline character.
	 * @param input the stream to get data from
	 * @param str the string to put the data into
	 * @return input
	 *
	 */
	std::istream& GetLine(std::istream& input, String& str)
	{
		return GetLine(input, str, input.widen('\n'));
	}

	/**
	 *
  	 * @brief (2) Read data from an I/O stream into a string
	 * @details GetLine reads characters from an input stream and places them into a string:@n
	 * 			Same as GetLine(input, str, input.widen('\n')), that is, the default delimiter is the endline character.
	 * @param input the stream to get data from
	 * @param str the string to put the data into
	 * @return input
	 *
	 */
	std::istream& GetLine(std::istream&& input, String& str)
	{
		return GetLine(std::move(input), str, input.widen('\n'));
	}

	/**
	 *
  	 * @brief (1) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @param base the number base
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int32_t StringToInt32(const String& str, size_t* pos, int32_t base)
	{
		assert(base >= 0 && base != 1 && base <= 36);
		if (pos != nullptr)
		{
			*pos = 0;
		}
		size_t index = 0;
		int32_t result = 0;
		for (; std::isspace(str.mString[index]); ++index)
		{
		}
		
		bool isNegative = false;
		if (str.mString[index] == '-' || str.mString[index] == '+')
		{
			isNegative = str.mString[index] == '-';
			++index;
			if (pos != nullptr)
			{
				++(*pos);
			}
		}

		if (base <= 10)
		{
			assert(('0' <= str.mString[index] && str.mString[index] <= '0' + base - 1));
		}
		else
		{
			assert(
				('0' <= str.mString[index] && str.mString[index] <= '9')
				|| ('a' <= str.mString[index] && str.mString[index] <= 'a' + base - 11)
				|| ('A' <= str.mString[index] && str.mString[index] <= 'A' + base - 11)
			);
		}

		if (str.mString[index] == '0')
		{
			if (base == 8)
			{
				++index;
				if (pos != nullptr)
				{
					++(*pos);
				}
			}

			if (base == 16 && (str.mString[index + 1] == 'x' || str.mString[index + 1] == 'X'))
			{
				index += 2;
				if (pos != nullptr)
				{
					(*pos) += 2;
				}
			}
		}
		
		while (true)
		{
			int32_t digit = 0;
			if (base <= 10)
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '0' + base - 1)
				{
					digit = static_cast<int32_t>(str.mString[index] - '0');
				}
				else
				{
					break;
				}
			}
			else
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '9')
				{
					digit = static_cast<int32_t>(str.mString[index] - '0');
				}
				else if ('a' <= str.mString[index] && str.mString[index] <= 'a' + base - 11)
				{
					digit = static_cast<int32_t>(str.mString[index] - 'a');
				}
				else if ('A' <= str.mString[index] && str.mString[index] <= 'A' + base - 11)
				{
					digit = static_cast<int32_t>(str.mString[index] - 'A');
				}
				else
				{
					break;
				}
			}
			if (result == 0 && digit == 0)
			{
				continue;
			}
			assert(result < base * result + digit);

			result = base * result + digit;
			if (pos != nullptr)
			{
				++(*pos);
			}
			++index;
		}

		return isNegative ? -result : result;
	}

	/**
	 *
  	 * @brief (1) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int32_t StringToInt32(const String& str, size_t* pos)
	{
		return StringToInt32(str, pos, 10);
	}

	/**
	 *
  	 * @brief (1) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int32_t StringToInt32(const String& str)
	{
		return StringToInt32(str, nullptr, 10);
	}

	/**
	 *
  	 * @brief (2) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @param base the number base
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int64_t StringToInt64(const String& str, size_t* pos, int32_t base)
	{
		assert(base >= 0 && base != 1 && base <= 36);
		if (pos != nullptr)
		{
			*pos = 0;
		}
		size_t index = 0;
		int64_t result = 0l;
		for (; std::isspace(str.mString[index]); ++index)
		{
		}
		
		bool isNegative = false;
		if (str.mString[index] == '-' || str.mString[index] == '+')
		{
			isNegative = str.mString[index] == '-';
			++index;
			if (pos != nullptr)
			{
				++(*pos);
			}
		}

		if (base <= 10)
		{
			assert(('0' <= str.mString[index] && str.mString[index] <= '0' + base - 1));
		}
		else
		{
			assert(
				('0' <= str.mString[index] && str.mString[index] <= '9')
				|| ('a' <= str.mString[index] && str.mString[index] <= 'a' + base - 11)
				|| ('A' <= str.mString[index] && str.mString[index] <= 'A' + base - 11)
			);
		}

		if (str.mString[index] == '0')
		{
			if (base == 8)
			{
				++index;
				if (pos != nullptr)
				{
					++(*pos);
				}
			}

			if (base == 16 && (str.mString[index + 1] == 'x' || str.mString[index + 1] == 'X'))
			{
				index += 2;
				if (pos != nullptr)
				{
					(*pos) += 2;
				}
			}
		}
		
		while (true)
		{
			int64_t digit = 0;
			if (base <= 10)
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '0' + base - 1)
				{
					digit = static_cast<int64_t>(str.mString[index] - '0');
				}
				else
				{
					break;
				}
			}
			else
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '9')
				{
					digit = static_cast<int64_t>(str.mString[index] - '0');
				}
				else if ('a' <= str.mString[index] && str.mString[index] <= 'a' + base - 11)
				{
					digit = static_cast<int64_t>(str.mString[index] - 'a');
				}
				else if ('A' <= str.mString[index] && str.mString[index] <= 'A' + base - 11)
				{
					digit = static_cast<int64_t>(str.mString[index] - 'A');
				}
				else
				{
					break;
				}
			}
			assert(result < static_cast<int64_t>(base) * result + digit);

			if (result == 0l && digit == 0l)
			{
				continue;
			}

			result = static_cast<int64_t>(base) * result + digit;
			if (pos != nullptr)
			{
				++(*pos);
			}
			++index;
		}

		return isNegative ? -result : result;
	}

	/**
	 *
  	 * @brief (2) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int64_t StringToInt64(const String& str, size_t* pos)
	{
		return StringToInt64(str, pos, 10);
	}

	/**
	 *
  	 * @brief (2) Converts a string to a signed integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	int64_t StringToInt64(const String& str)
	{
		return StringToInt64(str, nullptr, 10);
	}

	/**
	 *
  	 * @brief (1) Converts a string to a unsigned integer
	 * @details Interprets a unsigned integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) unsigned integer number representation 
	 * 			and converts them to an integer value. The valid unsigned integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type, 
	 * 			which applies unsigned integer wraparound rules. @n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @param base the number base
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint32_t StringToUint32(const String& str, size_t* pos, int32_t base)
	{
		return static_cast<uint32_t>(StringToInt32(str, pos, base));
	}

	/**
	 *
  	 * @brief (1) Converts a string to a unsigned integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint32_t StringToUint32(const String& str, size_t* pos)
	{
		return StringToUint32(str, pos, 10);
	}

	/**
	 *
  	 * @brief (1) Converts a string to a unsigned integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint32_t StringToUint32(const String& str)
	{
		return StringToUint32(str, nullptr, 10);
	}

	/**
	 *
  	 * @brief (2) Converts a string to a unsigned integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @param base the number base
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint64_t StringToUint64(const String& str, size_t* pos, int32_t base)
	{
		return static_cast<uint64_t>(StringToInt64(str, pos, base));
	}

	/**
	 *
  	 * @brief (2) Converts a string to a unsigned integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * 			If pos is not a null pointer, 
	 * 			then a pointer ptr - internal to the conversion functions - will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, giving the number of characters that were processed by the conversion.
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint64_t StringToUint64(const String& str, size_t* pos)
	{
		return StringToUint64(str, pos, 10);
	}

	/**
	 *
  	 * @brief (2) Converts a string to a unsigned integer
	 * @details Interprets a signed integer value in the string str.@n
	 * 			Discards any whitespace characters (as identified by calling isspace()) until the first non-whitespace character is found, 
	 * 			then takes as many characters as possible to form a valid base-n (where n=base) integer number representation 
	 * 			and converts them to an integer value. The valid integer value consists of the following parts: @n
	 * 			* (optional) plus or minus sign@n
	 * 			* (optional) prefix (0) indicating octal base (applies only when the base is 8 or ​0​)@n
	 * 			* (optional) prefix (0x or 0X) indicating hexadecimal base (applies only when the base is 16 or ​0​)@n
	 * 			* a sequence of digits @n
	 * 			The set of valid values for base is {0,2,3,...,36}. 
	 * 			The set of valid digits for base-2 integers is {0,1}, for base-3 integers is {0,1,2}, and so on. 
	 * 			For bases larger than 10, valid digits include alphabetic characters, 
	 * 			starting from Aa for base-11 integer, to Zz for base-36 integer. 
	 * 			The case of the characters is ignored.@n
	 * 			Additional numeric formats may be accepted by the currently installed C locale.@n
	 * 			If the value of base is ​0​, the numeric base is auto-detected: 
	 * 			if the prefix is 0, the base is octal, if the prefix is 0x or 0X, the base is hexadecimal, otherwise the base is decimal.@n
	 * 			If the minus sign was part of the input sequence, 
	 * 			the numeric value calculated from the sequence of digits is negated as if by unary minus in the result type.@n
	 * @param str the string to convert
	 * @return Integer value corresponding to the content of str.
	 *
	 */
	uint64_t StringToUint64(const String& str)
	{
		return StringToUint64(str, nullptr, 10);
	}

	/**
	 *
  	 * @brief (1) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * 			If pos is not a null pointer, then a pointer ptr, internal to the conversion functions, 
	 * 			will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, 
	 * 			giving the number of characters that were processed by the conversion. 
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return The string converted to the specified floating point type.
	 *
	 */
	float StringToFloat(const String& str, size_t* pos)
	{
		return static_cast<float>(StringToLongDouble(str, pos));
	}

	/**
	 *
  	 * @brief (1) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * @param str the string to convert
	 * @return The string converted to the specified floating point type.
	 *
	 */
	float StringToFloat(const String& str)
	{
		return StringToFloat(str, nullptr);
	}

	/**
	 *
  	 * @brief (2) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * 			If pos is not a null pointer, then a pointer ptr, internal to the conversion functions, 
	 * 			will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, 
	 * 			giving the number of characters that were processed by the conversion. 
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return The string converted to the specified floating point type.
	 *
	 */
	double StringToDouble(const String& str, size_t* pos)
	{
		return static_cast<double>(StringToLongDouble(str, pos));
	}

	/**
	 *
  	 * @brief (2) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * @param str the string to convert
	 * @return The string converted to the specified floating point type.
	 *
	 */
	double StringToDouble(const String& str)
	{
		return StringToDouble(str, nullptr);
	}

	/**
	 *
  	 * @brief (3) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * 			If pos is not a null pointer, then a pointer ptr, internal to the conversion functions, 
	 * 			will receive the address of the first unconverted character in str.GetCString(), 
	 * 			and the index of that character will be calculated and stored in *pos, 
	 * 			giving the number of characters that were processed by the conversion. 
	 * @param str the string to convert
	 * @param pos address of an integer to store the number of characters processed
	 * @return The string converted to the specified floating point type.
	 *
	 */
	long double StringToLongDouble(const String& str, size_t* pos)
	{
		if (pos != nullptr)
		{
			*pos = 0;
		}
		size_t index = 0;
		long double result = 0l;
		for (; std::isspace(str.mString[index]); ++index)
		{
		}
		
		// PLUS OR MINUS SIGN
		bool isNegative = false;
		if (str.mString[index] == '-' || str.mString[index] == '+')
		{
			isNegative = str.mString[index] == '-';
			++index;
			if (pos != nullptr)
			{
				++(*pos);
			}
		}

		assert(
			('0' <= str.mString[index] && str.mString[index] <= '9')
			|| ('a' <= str.mString[index] && str.mString[index] <= 'f')
			|| ('A' <= str.mString[index] && str.mString[index] <= 'F')
			|| (str.mString[index] == '.')
		);

		// HEXADECIMAL or INFINITY EXPRESSION or NOT-A-NUMBER EXPRESSION
		bool isHexadecimal = false;
		long double base = 10.0l;
		if (str.mString[index] == '0')
		{
			if (str.mString[index + 1] == 'x' || str.mString[index + 1] == 'X')
			{
				index += 2;
				isHexadecimal = true;
				base = 16.0f;
				if (pos != nullptr)
				{
					(*pos) += 2;
				}
			}
		}
		else if
		(
			(
				(strncmp(&str.mString[index], "INF", 3) == 0
				|| strncmp(&str.mString[index], "inf", 3) == 0) && std::isspace(str.mString[index + 3])
			)
			||
			(
				(strncmp(&str.mString[index], "INFINITY", 8) == 0
				|| strncmp(&str.mString[index], "infinity", 8) == 0) 
				&& std::isspace(str.mString[index + 8])
			)
		)
		{
			result = INFINITY;
		}
		else if
		(
			((strncmp(&str.mString[index], "NAN", 3) == 0
			|| strncmp(&str.mString[index], "nan", 3) == 0))
			&& (('0' <= str.mString[index + 3] && str.mString[index + 3] <= '9')
			|| ('a' <= str.mString[index + 3] && str.mString[index + 3] <= 'z')
			|| ('A' <= str.mString[index + 3] && str.mString[index + 3] <= 'Z')
			|| (str.mString[index + 3] == '_'))
		)
		{
			result = NAN;
		}
		
		bool isFractionalPart = false;
		bool isExponential = false;
		int32_t exponential = 0;
		bool isNegativeExponential = false;
		long double multiplier = base;
		while (result != INFINITY || result != NAN)
		{
			uint8_t digit = 0;
			if (!isFractionalPart && str.mString[index] == '.')
			{
				isFractionalPart = true;
				++index;
				if (pos != nullptr)
				{
					++(*pos);
				}
				continue;
			}

			if (
				!isExponential && (str.mString[index] == 'e' || str.mString[index] == 'E')
				&& (str.mString[index + 1] == '+' || str.mString[index + 1] == '-')
			)
			{
				isExponential = true;
				isNegativeExponential = str.mString[index + 1] == '-';
				index += 2;
				if (pos != nullptr)
				{
					(*pos) += 2;
				}
				continue;
			}

			if (!isHexadecimal)
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '9')
				{
					digit = str.mString[index] - '0';
				}
				else
				{
					break;
				}
			}
			else
			{
				if ('0' <= str.mString[index] && str.mString[index] <= '9')
				{
					digit = str.mString[index] - '0';
				}
				else if ('a' <= str.mString[index] && str.mString[index] <= 'f')
				{
					digit = str.mString[index] - 'a';
				}
				else if ('A' <= str.mString[index] && str.mString[index] <= 'F')
				{
					digit = str.mString[index] - 'A';
				}
				else
				{
					break;
				}
			}
			if
			(
				(!isExponential && result == 0.0f && digit == 0)
				|| (isExponential && exponential == 0 && digit == 0)
			)
			{
				++index;
				if (pos != nullptr)
				{
					++(*pos);
				}
				continue;
			}

			if (!isFractionalPart && !isExponential)
			{
				assert(result < base * result + digit);

				result = base * result + digit;
			}
			else if (isFractionalPart && !isExponential)
			{
				result = result + static_cast<long double>(digit) / multiplier;
				multiplier *= base;
			}
			else
			{
				exponential = exponential * static_cast<int32_t>(base) + static_cast<int32_t>(digit);
			}

			if (pos != nullptr)
			{
				++(*pos);
			}
			++index;
		}

		multiplier = base;
		for (int32_t i = 0; i < exponential; ++i)
		{
			if (isNegativeExponential)
			{
				result /= base;
			}
			else
			{
				result *= base;
			}
		}

		return isNegative ? -result : result;
	}

	/**
	 *
  	 * @brief (3) Converts a string to a floating point value
	 * @details Interprets a floating point value in a string str. @n
	 * 			Function discards any whitespace characters (as determined by std::isspace()) until first non-whitespace character is found. 
	 * 			Then it takes as many characters as possible to form a valid floating-point representation and converts them to a floating-point value. 
	 * 			The valid floating-point value can be one of the following: @n
	 * 			* decimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign
	 * 				* nonempty sequence of decimal digits optionally containing decimal-point character 
	 * 					(as determined by the current C locale) (defines significand)@n
	 * 				* (optional) e or E followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 10) @n
	 * 			* hexadecimal floating-point expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* 0x or 0X @n
	 * 				* nonempty sequence of hexadecimal digits optionally containing a decimal-point character 
	 * 					(as determined by the current C locale) (defines significand) @n
	 * 				* (optional) p or P followed with optional minus or plus sign and nonempty sequence of decimal digits (defines exponent to base 2) @n
	 * 			* infinity expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* INF or INFINITY ignoring case @n
	 * 			* not-a-number expression. It consists of the following parts: @n
	 * 				* (optional) plus or minus sign @n
	 * 				* NAN or NAN(char_sequence) ignoring case of the NAN part. char_sequence can only contain digits, 
	 * 					Latin letters, and underscores. The result is a quiet NaN floating-point value. @n
	 * 			* any other expression that may be accepted by the currently installed C locale @n
	 * @param str the string to convert
	 * @return The string converted to the specified floating point type.
	 *
	 */
	long double StringToLongDouble(const String& str)
	{
		return StringToLongDouble(str, nullptr);
	}

	/**
	 *
  	 * @brief (1) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts a signed integer to a string with the same content as what 
	 * 			std::sprintf(buf, "%d", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(int32_t value)
	{
		int32_t tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10)
		{
			tempValue /= 10;
			++size;
		}

		size_t sLength = size + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%d", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}
	
	/**
	 *
  	 * @brief (2) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts a signed integer to a string with the same content as what 
	 * 			std::sprintf(buf, "%ld", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(int64_t value)
	{
		int64_t tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10l)
		{
			tempValue /= 10l;
			++size;
		}

		size_t sLength = size + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%ld", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief (3) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts an unsigned integer to a string with the same content as what 
	 * 			std::sprintf(buf, "%u", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(uint32_t value)
	{
		uint32_t tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10u)
		{
			tempValue /= 10u;
			++size;
		}

		size_t sLength = size + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%u", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief (4) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts an unsigned integer to a string with the same content as what 
	 * 			std::sprintf(buf, "%lu", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(uint64_t value)
	{
		uint64_t tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10ul)
		{
			tempValue /= 10ul;
			++size;
		}

		size_t sLength = size + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%lu", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief (5) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts a floating point value to a string with the same content as what 
	 * 			std::sprintf(buf, "%f", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(float value)
	{
		float tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10.0f)
		{
			tempValue /= 10.0f;
			++size;
		}

		size_t sLength = size + 1ul + 6ul + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%f", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief (6) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts a floating point value to a string with the same content as what 
	 * 			std::sprintf(buf, "%f", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(double value)
	{
		double tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10.0)
		{
			tempValue /= 10.0;
			++size;
		}

		size_t sLength = size + 1ul + 6ul + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%f", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief (7) Converts an integral or floating point value to string
	 * @details Converts a numeric value to String. @n
	 * 			Converts a floating point value to a string with the same content as what 
	 * 			std::sprintf(buf, "%Lf", value) would produce for sufficiently large buf.
	 * @param value a numeric value to convert
	 * @return a string holding the converted value
	 *
	 */
	String ToString(long double value)
	{
		long double tempValue = value;
		size_t size = 1ul;
		while (tempValue >= 10.0l)
		{
			tempValue /= 10.0l;
			++size;
		}

		size_t sLength = size + 1ul + 6ul + 1ul;
		char* s = reinterpret_cast<char*>(Memory::Malloc(sLength * sizeof(char)));
		Memory::Memset(s, 0, sLength);
		snprintf(s, sLength, "%Lf", value);

		String result = String{s};
		Memory::Free(s);
		
		return result;
	}

	/**
	 *
  	 * @brief Converts a character array literal to String
	 * @details Forms a string literal of the desired type. @n
	 * 			Returns String{str, len}
	 * @param str pointer to the beginning of the raw character array literal
	 * @param len length of the raw character array literal 
	 * @return The string literal.
	 *
	 */
	String operator""_s(const char* str, size_t len)
	{
		return String{str, len};
	}

#ifdef CAVE_BUILD_DEBUG
	namespace StringTest
	{
		void Test()
		{
			// LogManager::SetVerbosity(eLogVerbosity::Error);
			Constructor();
			AssignmentOperator();
			SubscriptOperator();
			GetFront();
			GetBack();
			GetCString();
			IsEmpty();
			GetSize();
			SetCapacity();
			GetCapacity();
			Shrink();
			Clear();
			InsertAt();
			RemoveAt();
			Append();
			AdditionCompoundAssignmentOperator();
			StartsWith();
			EndsWith();
			Contains();
			Replace();
			GetSubstring();
			Resize();
			GetIndexOf();
			GetLastIndexOf();
			AdditionOperator();
			ComparisonOperator();
			StreamOperator();
			// GetLine();
			StringToInt();
			StringToFloat();
			ToString();
		}

		void Constructor()
		{
			{
				String s;
				assert(s.IsEmpty() && (s.GetLength() == 0));
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String() TEST SUCCESS");
			}

			{
				String s(4, '=');
				assert(strncmp(s.GetCString(), "====", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(size_t count, char ch) TEST SUCCESS");
			}

			{
				String const other("Exemplary");
				String s(other, 0, other.GetLength() - 1);
				LOGDF(eLogChannel::CORE_STRING, std::cout, "other: %s, s: %s", other.GetCString(), s.GetCString());
				assert(strncmp(s.GetCString(), "Exemplar", s.GetLength()) == 0);
			}

			{
				String const other("Mutatis Mutandis");
				String s(other, 8);
				assert(strncmp(s.GetCString(), "Mutandis", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(const String& other, size_t pos) TEST SUCCESS");
			}

			{
				String s("C-style string", 7);
				assert(strncmp(s.GetCString(), "C-style", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(char const* s, size_t count) TEST SUCCESS");
			}

			{
				String s("C-style\0string");
				assert(strncmp(s.GetCString(), "C-style", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(char const* s) TEST SUCCESS");
			}

			{
				char mutable_c_str[] = "another C-style string";
				String s(mutable_c_str + 8, mutable_c_str + Strlen(mutable_c_str) - 1);
				assert(strncmp(s.GetCString(), "C-style string", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(const char* first, const char* last) TEST SUCCESS");
			}

			{
				String const other("Exemplar");
				String s(other);
				assert(strncmp(s.GetCString(), "Exemplar", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(const String& other) TEST SUCCESS");
			}

			{
				String s(String("C++ by ") + String("example"));
				assert(s.GetLength() == Strlen("C++ by example"));
				assert(strncmp(s.GetCString(), "C++ by example", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(String&& str) TEST SUCCESS");
			}

			{
				// overload resolution selects String(InputIt first, InputIt last) [with InputIt = int]
				// which behaves as if String(size_t count, char ch) is called
				String s(3, std::toupper('a'));
				assert(strncmp(s.GetCString(), "AAA", s.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String::String(size_t count, char ch) TEST SUCCESS");
			}
		}

		void AssignmentOperator()
		{
			String str1;
			String str2 { "alpha" };
		
			str1 = str2;
			assert(strncmp(str1.GetCString(), "alpha", str1.GetLength()) == 0);
			assert(strncmp(str2.GetCString(), "alpha", str1.GetLength()) == 0);
			assert(strncmp(str1.GetCString(), str2.GetCString(), str1.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator=(const String&) TEST SUCCESS");

			str1 = std::move(str2);
			assert(strncmp(str1.GetCString(), "alpha", str1.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator=(String&&) TEST SUCCESS");
		
			str1 = "beta";
			assert(strncmp(str1.GetCString(), "beta", str1.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator=(const char*) TEST SUCCESS");
		
			str1 = '!'; 
			assert(strncmp(str1.GetCString(), "!", str1.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator=(char) TEST SUCCESS");
		}

		void SubscriptOperator()
		{
			String const e("Exemplar");
			for (size_t i = e.GetLength() - 1, j = 0; i != 0; i /= 2, ++j)
			{
				assert(e[i] == "rmx"[j]);
			}
		
			const char* c = &e[0];
			assert(strncmp(c, e.GetCString(), e.GetLength()) == 0);
		
			// Change the last character of s into a 'y'
			String s("Exemplar ");
			s[s.GetLength() - 1] = 'y'; // equivalent to s.GetBack() = 'y';
			assert(strncmp(s.GetCString(), "Exemplary", s.GetLength()) == 0);

			LOGD(eLogChannel::CORE_STRING, std::cout, "String::operator[](size_t pos) TEST SUCCESS");
		}

		void GetFront()
		{
			{
				String s("Exemplary");
				char& f = s.GetFront();
				f = 'e';
				assert(strncmp(s.GetCString(), "exemplary", s.GetLength()) == 0);
			}
			
			{
				String const c("Exemplary");
				char const& f = c.GetFront();
				assert(strncmp(&f, "Exemplary", Strlen(&f)) == 0);
			}

			LOGD(eLogChannel::CORE_STRING, std::cout, "char& String::GetFront() TEST SUCCESS");
		}

		void GetBack()
		{
			{
				String s("Exemplary");
				char& back = s.GetBack();
				back = 's';
				assert(strncmp(s.GetCString(), "Exemplars", s.GetLength()) == 0);
			}
			
			{
				String const c("Exemplary");
				char const& back = c.GetBack();
				assert(back == 'y');
			}

			LOGD(eLogChannel::CORE_STRING, std::cout, "char& String::GetBack() TEST SUCCESS");
		}

		void GetCString()
		{
			String const s("Emplary");
			assert(s.GetLength() == Strlen(s.GetCString()));
			assert(0 == *(s.GetCString() + s.GetLength()));

			LOGD(eLogChannel::CORE_STRING, std::cout, "const char* String::GetCString() TEST SUCCESS");
		}

		void IsEmpty()
		{
			String s;
			assert(s.IsEmpty());
			assert(strncmp(s.GetCString(), "", 1ul) == 0);
		
			s = "Exemplar";
			assert(!s.IsEmpty());
			assert(strncmp(s.GetCString(), "Exemplar", s.GetLength()) == 0);
		
			s = "";
			assert(s.IsEmpty());
			assert(strncmp(s.GetCString(), "", 1ul) == 0);

			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::IsEmpty() TEST SUCCESS");
		}

		void GetSize()
		{
			String s("Exemplar");
			assert(8ul == s.GetLength());

			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetSize() TEST SUCCESS");
		}

		void SetCapacity()
		{
			String s;
			size_t newCapacity = 100ul;
			assert(newCapacity > s.GetCapacity());

			s.SetCapacity(newCapacity);
			assert(newCapacity <= s.GetCapacity());

			LOGD(eLogChannel::CORE_STRING, std::cout, "void String::SetCapacity() TEST SUCCESS");
		}
		
		void GetCapacity()
		{
			String s{"Exemplar"};
			assert(s.GetCapacity() == 15ul);
		
			s += " is an example string.";
			assert(s.GetCapacity() == 31ul);

			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetCapacity() TEST SUCCESS");
		}
	
		void Shrink()
		{
			String s;
			assert(s.GetCapacity() == String::ALIGNED_BYTE - 1ul);
			assert(s.GetLength() == 0ul);

			for (int i = 0; i < 42; i++)
			{
				s.Append(" 42 ");
			}

			assert(s.GetCapacity() == 255ul);
			assert(s.GetLength() == 168ul);
			
			s.Clear();
			assert(s.GetCapacity() == 255ul);
			assert(s.GetLength() == 0ul);

			s.Shrink();
			assert(s.GetCapacity() == String::ALIGNED_BYTE - 1ul);
			assert(s.GetLength() == 0ul);

			LOGD(eLogChannel::CORE_STRING, std::cout, "void String::Shrink() TEST SUCCESS");
		}

		void Clear()
		{
			String s{ "Exemplar" };
			size_t const capacity = s.GetCapacity();
		
			s.Clear();
			assert(s.GetCapacity() == capacity);
			assert(s.IsEmpty());
			assert(s.GetLength() == 0);

			LOGD(eLogChannel::CORE_STRING, std::cout, "void String::Clear() TEST SUCCESS");
		}
	
		void InsertAt()
		{
			String s = "xmplr";
 
			s.InsertAt(0, 1, 'E');
			assert(strncmp("Exmplr", s.GetCString(), s.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::InsertAt(size_t index, size_t count, char ch) TEST SUCCESS");
		
			s.InsertAt(2, "e");
			assert(strncmp("Exemplr", s.GetCString(), s.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::InsertAt(size_t index, const char* s) TEST SUCCESS");
		
			String a = "a";
			s.InsertAt(6, a);
			assert(strncmp("Exemplar", s.GetCString(), s.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::InsertAt(size_t index, String const& str) TEST SUCCESS");
		
			String exampleStr = " is an example string.";
			s.InsertAt(8, exampleStr, 0, 14);
			assert(strncmp("Exemplar is an example", s.GetCString(), s.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::InsertAt(size_t index, String const& str, size_t indexStr, size_t count) TEST SUCCESS");
		}

		void RemoveAt()
		{
			String s = "This is an example";
			assert(strncmp(s.GetCString(), "This is an example", s.GetLength()) == 0);

			s.RemoveAt(0, 5); // Erase "This "
			assert(strncmp(s.GetCString(), "is an example", s.GetLength()) == 0);

			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::RemoveAt(size_t index = 0, size_t count) TEST SUCCESS");
		}

		void Append()
		{
			String str = "string";
			const char* cPtr = "C-string";
			const char cArr[] = "Two and one";
		
			String output;
		
			// 1) Append a char 3 times. 
			// Notice, this is the only overload accepting chars.
			output.Append(3, '*');
			assert(strncmp(output.GetCString(), "***", output.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Append(size_t count, char ch) TEST SUCCESS");
		
			//  2) Append a whole string
			output.Append(str);
			assert(strncmp(output.GetCString(), "***string", output.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Append(const String& str) TEST SUCCESS");
		
			// 3) Append part of a string (last 3 letters, in this case)
			output.Append(str, 3, 3);
			assert(strncmp(output.GetCString(), "***stringing", output.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Append(const String& str, size_t pos, size_t count) TEST SUCCESS");
		
			// 4) Append part of a C-string
			output.Append(1, ' ');
			assert(strncmp(output.GetCString(), "***stringing ", output.GetLength()) == 0);
			output.Append(cArr, 4);
			assert(strncmp(output.GetCString(), "***stringing Two ", output.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Append(const char* s, size_t count) TEST SUCCESS");
		
			// 5) Append a whole C-string
			output.Append(cPtr);
			assert(strncmp(output.GetCString(), "***stringing Two C-string", output.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Append(const char* s, size_t count) TEST SUCCESS");
		}

		void AdditionCompoundAssignmentOperator()
		{
			String str;
			str.SetCapacity(50); //reserves sufficient storage space to avoid memory reallocation
			assert(strncmp(str.GetCString(), "", 1ul) == 0);
			
			str += "This";
			assert(strncmp(str.GetCString(), "This", str.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator+=(const char* s) TEST SUCCESS");
			
			str += String(" is ");
			assert(strncmp(str.GetCString(), "This is ", str.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator+=(const String& str) TEST SUCCESS");
			
			str += 'a';
			assert(strncmp(str.GetCString(), "This is a", str.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::operator+=(char ch) TEST SUCCESS");
		}

		void StartsWith()
		{
			String helloWorld = String("hello world");
 
			assert(helloWorld.StartsWith(String("hello")));
			assert(!helloWorld.StartsWith(String("goodbye")));
			assert(helloWorld.StartsWith('h'));
			assert(!helloWorld.StartsWith('x'));

			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::StartsWith TEST SUCCESS");
		}

		void EndsWith()
		{
			String helloWorld = String("hello world");
 
			assert(helloWorld.EndsWith(String("world")));
			assert(!helloWorld.EndsWith(String("goodbye")));
			assert(helloWorld.EndsWith('d'));
			assert(!helloWorld.EndsWith('x'));

			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::EndsWith TEST SUCCESS");
		}

		void Contains()
		{
			String helloWorld = String("hello world");
 
			assert(helloWorld.Contains(String("hello")));
			assert(!helloWorld.Contains(String("goodbye")));
			assert(helloWorld.Contains('w'));
			assert(!helloWorld.Contains('x'));

			LOGD(eLogChannel::CORE_STRING, std::cout, "bool String::Contains TEST SUCCESS");
		}

		void Replace()
		{
			{
				String str = "hello world";

				str.Replace(0, 1, String("bey"));
				assert(str.GetLength() == Strlen("beyello world"));
				assert(strncmp(str.GetCString(), "beyello world", str.GetLength()) == 0);

				str.Replace(str.GetLength(), 1, String("boy"));
				assert(str.GetLength() == Strlen("beyello worldboy"));
				assert(strncmp(str.GetCString(), "beyello worldboy", str.GetLength()) == 0);

				str.Replace(2, 2, String(""));
				assert(str.GetLength() == Strlen("bello worldboy"));
				assert(strncmp(str.GetCString(), "bello worldboy", str.GetLength()) == 0);	
				LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::Replace(size_t pos, size_t count, const String& str) TEST SUCCESS");
			}

			{
				String str = "hello world";

				str.Replace(0, 1, String("bey"), 1, 1);
				assert(str.GetLength() == Strlen("eello world"));
				assert(strncmp(str.GetCString(), "eello world", str.GetLength()) == 0);

				str.Replace(str.GetLength(), 1, String("boy"), 3, 1);
				assert(str.GetLength() == Strlen("eello world"));
				assert(strncmp(str.GetCString(), "eello world", str.GetLength()) == 0);

				str.Replace(0, 5, String("good morning, hello"), 14, 5);
				assert(str.GetLength() == Strlen("hello world"));
				assert(strncmp(str.GetCString(), "hello world", str.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::Replace(size_t pos, size_t count, const String& str, size_t pos2, size_t count2 = NPOS) TEST SUCCESS");
			}

			{
				String str = "hello world";

				str.Replace(0, 1, "bey", 2);
				assert(str.GetLength() == Strlen("beello world"));
				assert(strncmp(str.GetCString(), "beello world", str.GetLength()) == 0);

				str.Replace(str.GetLength(), 1, "boy", 1);
				assert(str.GetLength() == Strlen("beello worldb"));
				assert(strncmp(str.GetCString(), "beello worldb", str.GetLength()) == 0);

				str.Replace(2, 2, "blyat", 0);
				assert(str.GetLength() == Strlen("belo worldb"));
				assert(strncmp(str.GetCString(), "belo worldb", str.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::Replace(size_t pos, size_t count, const char* cStr, size_t count2) TEST SUCCESS");
			}

			{
				String str = "hello world";

				str.Replace(0, 1, "bey");
				assert(str.GetLength() == Strlen("beyello world"));
				assert(strncmp(str.GetCString(), "beyello world", str.GetLength()) == 0);

				str.Replace(str.GetLength(), 1, "boy");
				assert(str.GetLength() == Strlen("beyello worldboy"));
				assert(strncmp(str.GetCString(), "beyello worldboy", str.GetLength()) == 0);

				str.Replace(2, 2, "");
				assert(str.GetLength() == Strlen("bello worldboy"));
				assert(strncmp(str.GetCString(), "bello worldboy", str.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::Replace(size_t pos, size_t count, const char* cStr) TEST SUCCESS");
			}

			{
				String str = "hello world";

				str.Replace(0, 1, 3, 'h');
				assert(str.GetLength() == Strlen("hhhello world"));
				assert(strncmp(str.GetCString(), "hhhello world", str.GetLength()) == 0);

				str.Replace(str.GetLength(), 2, 4, 'd');
				assert(str.GetLength() == Strlen("hhhello worlddddd"));
				assert(strncmp(str.GetCString(), "hhhello worlddddd", str.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String& String::Replace(size_t pos, size_t count, size_t count2, char ch) TEST SUCCESS");
			}
		}

		void GetSubstring()
		{
			String a = "0123456789abcdefghij";
 
			// count is npos, returns [pos, GetLength())
			String sub1 = a.GetSubstring(10);
			assert(sub1.GetLength() == Strlen("abcdefghij"));
			assert(strncmp(sub1.GetCString(), "abcdefghij", sub1.GetLength()) == 0);
		
			// both pos and pos+count are within bounds, returns [pos, pos+count)
			String sub2 = a.GetSubstring(5, 3);
			assert(sub2.GetLength() == Strlen("567"));
			assert(strncmp(sub2.GetCString(), "567", sub2.GetLength()) == 0);
		
			// pos is within bounds, pos+count is not, returns [pos, GetLength()) 
			String sub4 = a.GetSubstring(a.GetLength() - 3, 50);
			// this is effectively equivalent to
			// String sub4 = a.GetSubstring(17, 3);
			// since a.GetLength() == 20, pos == a.GetLength()-3 == 17, and a.GetLength()-pos == 3
		
			assert(sub4.GetLength() == Strlen("hij"));
			assert(strncmp(sub4.GetCString(), "hij", sub4.GetLength()) == 0);
		
			LOGD(eLogChannel::CORE_STRING, std::cout, "String String::GetSubstring(size_t pos, size_t count) TEST SUCCESS");
		}

		void Resize()
		{
			const size_t desired_length = 8ul;
			String longString("Where is the end?");
			String shortString("Ha");

			// Shorten
			longString.Resize(desired_length);
			assert(longString.GetLength() == Strlen("Where is"));
			assert(strncmp(longString.GetCString(), "Where is", longString.GetLength()) == 0);
			
			// Lengthen
			shortString.Resize(desired_length, 'a');
			assert(shortString.GetLength() == Strlen("Haaaaaaa"));
			assert(strncmp(shortString.GetCString(), "Haaaaaaa", shortString.GetLength()) == 0);

			LOGD(eLogChannel::CORE_STRING, std::cout, "void String::Resize(size_t count, char ch) TEST SUCCESS");
		}
	
		void GetIndexOf()
		{
			int32_t n = 0ul;
			String const s = "This is a string";
		
			// search from beginning of string
			n = s.GetIndexOf("is");
			assert(static_cast<size_t>(n) != String::NPOS);
			String sub1 = s.GetSubstring(n);
			assert(sub1.GetLength() == Strlen("is is a string"));
			assert(strncmp(sub1.GetCString(), "is is a string", sub1.GetLength()) == 0);
		
			// search from position 5
			n = s.GetIndexOf("is", 5);
			assert(static_cast<size_t>(n) != String::NPOS);
			String sub2 = s.GetSubstring(n);
			assert(sub2.GetLength() == Strlen("is a string"));
			assert(strncmp(sub2.GetCString(), "is a string", sub2.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetIndexOf(const char* s, size_t pos) TEST SUCCESS");
		
			// find a single character
			n = s.GetIndexOf('a');
			assert(static_cast<size_t>(n) != String::NPOS);
			String sub3 = s.GetSubstring(n);
			assert(sub3.GetLength() == Strlen("a string"));
			assert(strncmp(sub3.GetCString(), "a string", sub3.GetLength()) == 0);
		
			// find a single character
			n = s.GetIndexOf('q');
			assert(static_cast<size_t>(n) == String::NPOS);
			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetIndexOf(char ch, size_t pos) TEST SUCCESS");
		}

		void GetLastIndexOf()
		{
			size_t n = 0ul;
			String const s = "This is a string";
		
			// search backwards from end of string
			n = s.GetLastIndexOf("is");
			assert(n != String::NPOS && n == 5);
			String sub1 = s.GetSubstring(n);
			assert(sub1.GetLength() == Strlen("is a string"));
			assert(strncmp(sub1.GetCString(), "is a string", sub1.GetLength()) == 0);

			// search backwards from position 4
			n = s.GetLastIndexOf("is", 4);
			assert(n != String::NPOS && n == 2);
			String sub2 = s.GetSubstring(n);
			assert(sub2.GetLength() == Strlen("is is a string"));
			assert(strncmp(sub2.GetCString(), "is is a string", sub2.GetLength()) == 0);
			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetIndexOf(const char* s, size_t pos) TEST SUCCESS");
			
			// find a single character
			n = s.GetLastIndexOf('s');
			assert(n != String::NPOS && n == 10);
			String sub3 = s.GetSubstring(n);
			assert(sub3.GetLength() == Strlen("string"));
			assert(strncmp(sub3.GetCString(), "string", sub3.GetLength()) == 0);

			// find a single character
			n = s.GetLastIndexOf('q');
			assert(n == String::NPOS);

			LOGD(eLogChannel::CORE_STRING, std::cout, "size_t String::GetIndexOf(char ch, size_t pos) TEST SUCCESS");
		}
	
		void AdditionOperator()
		{
			{
				String lhs = "hello ";
				String rhs = "world!";
				String result = lhs + rhs;
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const String& lhs, const String& rhs) TEST SUCCESS");
			}

			{
				String lhs = "hello ";
				const char* rhs = "world!";
				String result = lhs + rhs;
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const String& lhs, const char* rhs) TEST SUCCESS");
			}

			{
				String lhs = "hello";
				char rhs = '!';
				String result = lhs + rhs;
				assert(result.GetLength() == Strlen("hello!"));
				assert(strncmp(result.GetCString(), "hello!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const String& lhs, char rhs) TEST SUCCESS");
			}
			
			{
				const char* lhs = "hello ";
				String rhs = "world!";
				String result = lhs + rhs;
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const char* lhs, const String& rhs) TEST SUCCESS");
			}

			{
				char lhs = 'h';
				String rhs = "ello!";
				String result = lhs + rhs;
				assert(result.GetLength() == Strlen("hello!"));
				assert(strncmp(result.GetCString(), "hello!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(char lhs, const String& rhs) TEST SUCCESS");
			}

			{
				String lhs = "hello ";
				String rhs = "world!";
				String result = std::move(lhs) + std::move(rhs);
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(String&& lhs, String&& rhs) TEST SUCCESS");
			}

			{
				String lhs = "hello ";
				String rhs = "world!";
				String result = std::move(lhs) + rhs;
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(String&& lhs, const String& rhs) TEST SUCCESS");
			}
			
			{
				String lhs = "hello ";
				const char* rhs = "world!";
				String result = std::move(lhs) + rhs;
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(String&& lhs, const char* rhs) TEST SUCCESS");
			}
			
			{
				String lhs = "hello";
				char rhs = '!';
				String result = std::move(lhs) + rhs;
				assert(result.GetLength() == Strlen("hello!"));
				assert(strncmp(result.GetCString(), "hello!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(String&& lhs, char rhs) TEST SUCCESS");
			}
			
			{
				String lhs = "hello ";
				String rhs = "world!";
				String result = lhs + std::move(rhs);
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const String& lhs, String&& rhs) TEST SUCCESS");
			}
			
			{
				const char* lhs = "hello ";
				String rhs = "world!";
				String result = lhs + std::move(rhs);
				assert(result.GetLength() == Strlen("hello world!"));
				assert(strncmp(result.GetCString(), "hello world!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(const char* lhs, String&& rhs) TEST SUCCESS");
			}
			
			{
				char lhs = 'h';
				String rhs = "ello!";
				String result = lhs + std::move(rhs);
				assert(result.GetLength() == Strlen("hello!"));
				assert(strncmp(result.GetCString(), "hello!", result.GetLength()) == 0);
				LOGD(eLogChannel::CORE_STRING, std::cout, "String operator+(char lhs, String&& rhs) TEST SUCCESS");
			}
		}
	
		void ComparisonOperator()
		{
			{
				String lhs1("hello");
				String rhs1("hello");
				assert(lhs1 == rhs1);

				String lhs2("hello");
				String rhs2("hellm");
				assert(!(lhs2 == rhs2));

				String lhs3;
				String rhs3;
				assert(lhs3 == rhs3);

				String lhs4("");
				String rhs4("");
				assert(lhs4 == rhs4);

				String lhs5;
				String rhs5("");
				assert(lhs5 == rhs5);

				String lhs6("\0");
				String rhs6("");
				assert(lhs6 == rhs6);
				LOGD(eLogChannel::CORE_STRING, std::cout, "bool operator==(const String& lhs, const String& rhs) TEST SUCCESS");
			}

			{
				String lhs1("hello");
				const char* rhs1 = "hello";
				assert(lhs1 == rhs1);

				String lhs2("hello");
				const char* rhs2 = "hellm";
				assert(!(lhs2 == rhs2));

				String lhs3;
				const char* rhs3 = "";
				assert(lhs3 == rhs3);

				String lhs4("");
				const char* rhs4 = "";
				assert(lhs4 == rhs4);

				String lhs5("some");
				char rhs5[] = {'s', 'o', 'm', 'e'};
				assert(lhs5 == rhs5);

				String lhs6("\0");
				const char* rhs6 = "";
				assert(lhs6 == rhs6);

				LOGD(eLogChannel::CORE_STRING, std::cout, "bool operator==(const String& lhs, const char* rhs) TEST SUCCESS");
			}

			{
				String lhs1("hello");
				String rhs1("hell");
				assert(lhs1 != rhs1);

				String lhs2("hello");
				String rhs2("hellm");
				assert(lhs2 != rhs2);

				String lhs3;
				String rhs3;
				assert(!(lhs3 != rhs3));

				String lhs4(".");
				String rhs4("");
				assert(lhs4 != rhs4);

				String lhs5("\0");
				String rhs5("");
				assert(!(lhs5 != rhs5));
				
				LOGD(eLogChannel::CORE_STRING, std::cout, "bool operator!=(const String& lhs, const String& rhs) TEST SUCCESS");
			}

			{
				String lhs1("hello");
				const char* rhs1 = "hell";
				assert(lhs1 != rhs1);

				String lhs2("hello");
				const char* rhs2 = "hellm";
				assert(lhs2 != rhs2);

				String lhs3;
				const char* rhs3 = "";
				assert(!(lhs3 != rhs3));

				String lhs4("");
				const char* rhs4 = ".";
				assert(lhs4 != rhs4);

				String lhs5("some");
				char rhs5[] = {'s', 'o', 'm', 'm'};
				assert(lhs5 != rhs5);

				String lhs6("\0");
				const char* rhs6 = "";
				assert(!(lhs6 != rhs6));

				LOGD(eLogChannel::CORE_STRING, std::cout, "bool operator!=(const String& lhs, const char* rhs) TEST SUCCESS");
			}
		}
	
		void StreamOperator()
		{
			{
				String s = "hello";
				std::cout << s << std::endl;

				LOGD(eLogChannel::CORE_STRING, std::cout, "std::ostream& operator<<(std::ostream& os, const String& str) TEST SUCCESS");
			}

			// {
			// 	String s;
			// 	std::cin >> s;
			// 	std::cout << "s: " << s << std::endl;
			// 	LOGD(eLogChannel::CORE_STRING, std::cout, "std::istream& operator>>(std::istream& is, String& str) TEST SUCCESS");
			// }
		}
	
		void GetLine()
		{
			// greet the user
			String name;
			std::cout << "What is your name? ";
			GetLine(std::cin, name);
			std::cout << "Hello " << name << ", nice to meet you.\n";
		
			// read file line by line
			std::istringstream input;
			input.str("1\n2\n3\n4\n5\n6\n7\n");
			int32_t sum = 0;
			for (String line; GetLine(input, line); )
			{
				sum += StringToInt32(line);
			}
			assert(sum == 28);

			LOGD(eLogChannel::CORE_STRING, std::cout, "std::istream& GetLine(std::istream& input, String& str, char delim) TEST SUCCESS");
		}

		void StringToInt()
		{
			String str1 = "45";
			String str2 = "3.14159";
			String str3 = "31337 with words";
			String str4 = "words and 2";
		
			int32_t myint1 = StringToInt32(str1);
			int32_t myint2 = StringToInt32(str2);
			int32_t myint3 = StringToInt32(str3);
			// error
			// int myint4 = StringToInt32(str4);
		
			assert(myint1 == 45);
			assert(myint2 == 3);
			assert(myint3 == 31337);
			// std::cout << "StringToInt32(\"" << str4 << "\") is " << myint4 << '\n';

			LOGD(eLogChannel::CORE_STRING, std::cout, "int32_t StringToInt32(const String& str, size_t* pos, int32_t base) TEST SUCCESS");
		}
	
		void StringToFloat()
		{
			String orbits("686.97 365.24");
			size_t sz;

			float mars = StringToFloat(orbits, &sz);
			float earth = StringToFloat(orbits.GetSubstring(sz));

			assert(mars == 686.97f);
			assert(earth == 365.24f);

			LOGD(eLogChannel::CORE_STRING, std::cout, "int32_t StringToInt32(const String& str, size_t* pos, int32_t base) TEST SUCCESS");
		}

		void ToString()
		{
			double f = 23.43;
			double f2 = 1e-9;
			double f3 = 1e40;
			double f4 = 1e-40;
			double f5 = 123456789;
			String fStr = cave::ToString(f);
			String fStr2 = cave::ToString(f2); // Note: returns "0.000000"
			String fStr3 = cave::ToString(f3); // Note: Does not return "1e+40".
			String fStr4 = cave::ToString(f4); // Note: returns "0.000000"
			String fStr5 = cave::ToString(f5);

			assert(fStr == "23.430000");
			assert(fStr2 == "0.000000");
			assert(fStr3 == "10000000000000000303786028427003666890752.000000");
			assert(fStr4 == "0.000000");
			assert(fStr5 == "123456789.000000");

			LOGD(eLogChannel::CORE_STRING, std::cout, "String ToString(double value) TEST SUCCESS");
		}
	}
#endif
} // namespace cave
#endif