/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */

#pragma once

#ifdef __WIN32__
import String;
#else
#include <iostream>

#include "CoreTypes.h"
#include "CoreGlobals.h"
#include "Memory/MemoryPool.h"
#include "Utils/Crt.h"

namespace cave
{
	/**
	 *
	 * @brief String stores and manipulates sequences of <code>char</code> objects,
	 * 			which are non-array objects of
	 * 			[trivial](https://en.cppreference.com/w/cpp/named_req/TrivialType)
	 * 			[standard-layout](https://en.cppreference.com/w/cpp/named_req/StandardLayoutType) type.
	 * @details The class is dependent on the character type and the nature of operations on that type.
	 * 			@n@n
	 * 			The elements of a <code>String</code> are stored contiguously, that is, for as <code>String s</code>,
	 * 			<code>&*(s.GetCString() + n) == &*s.GetString() + n</code> for any <code>n</code> in [0, <code>s.GetLength()</code>],
	 * 			or equivalently, a pointer to <code>s[0]</code> can be passed to functions that expect a pointer to the first element of a null-terminated
	 * 			char[] array.
	 * @author Alegruz
	 * @date 2021-04-30
	 * @version 0.0.1
	 * @todo check if cave::String satisfies the requirements of
	 * 			[AllocatorAwareContainer](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer),
	 * 			[SequenceContainer](https://en.cppreference.com/w/cpp/named_req/SequenceContainer)</a> and
	 * 			[ContiguousContainer](https://en.cppreference.com/w/cpp/named_req/ContiguousContainer)</a>
	 *
	 */
	class String final
	{
		// Non-member functions
		friend String operator+(const String& lhs, const String& rhs);
		friend String operator+(const String& lhs, const char* rhs);
		friend String operator+(const String& lhs, char rhs);
		friend String operator+(const char* lhs, const String& rhs);
		friend String operator+(char lhs, const String& rhs);
		friend String operator+(String&& lhs, String&& rhs);
		friend String operator+(String&& lhs, const String& rhs);
		friend String operator+(String&& lhs, const char* rhs);
		friend String operator+(String&& lhs, char rhs);
		friend String operator+(const String& lhs, String&& rhs);
		friend String operator+(const char* lhs, String&& rhs);
		friend String operator+(char lhs, String&& rhs);
		friend constexpr bool operator==(const String& lhs, const String& rhs) noexcept;
		friend constexpr bool operator!=(const String& lhs, const String& rhs) noexcept;
		friend constexpr bool operator==(const String& lhs, const char* rhs) noexcept;
		friend constexpr bool operator!=(const String& lhs, const char* rhs) noexcept;
		// input / output
		friend std::ostream& operator<<(std::ostream& os, const String& str);
		friend std::istream& operator>>(std::istream& is, String& str);
		friend std::istream& GetLine(std::istream& input, String& str, char delim);
		friend std::istream& GetLine(std::istream&& input, String& str, char delim);
		friend std::istream& GetLine(std::istream& input, String& str);
		friend std::istream& GetLine(std::istream&& input, String& str);
		// numeric conversions
		friend int32_t StringToInt32(const String& str, size_t* pos, int32_t base);
		friend int32_t StringToInt32(const String& str, size_t* pos);
		friend int32_t StringToInt32(const String& str);
		friend int64_t StringToInt64(const String& str, size_t* pos, int32_t base);
		friend int64_t StringToInt64(const String& str, size_t* pos);
		friend int64_t StringToInt64(const String& str);
		friend uint32_t StringToUint32(const String& str, size_t* pos, int32_t base);
		friend uint32_t StringToUint32(const String& str, size_t* pos);
		friend uint32_t StringToUint32(const String& str);
		friend uint64_t StringToUint64(const String& str, size_t* pos, int32_t base);
		friend uint64_t StringToUint64(const String& str, size_t* pos);
		friend uint64_t StringToUint64(const String& str);
		friend float StringToFloat(const String& str, size_t* pos);
		friend float StringToFloat(const String& str);
		friend double StringToDouble(const String& str, size_t* pos);
		friend double StringToDouble(const String& str);
		friend long double StringToLongDouble(const String& str, size_t* pos);
		friend long double StringToLongDouble(const String& str);
	public:
		String() noexcept(noexcept(gCoreMemoryPool));
		explicit String(MemoryPool& pool) noexcept;
		String(size_t count, char ch, MemoryPool& pool = gCoreMemoryPool);
		String(const String& other, size_t pos, MemoryPool& pool = gCoreMemoryPool);
		String(const String& other, size_t pos, size_t count, MemoryPool& pool = gCoreMemoryPool);
		constexpr String(const char* s, size_t count, MemoryPool& pool = gCoreMemoryPool);
		constexpr String(const char* s, MemoryPool& pool = gCoreMemoryPool);
		constexpr String(const char* first, const char* last, MemoryPool& pool = gCoreMemoryPool);
		constexpr String(const String& other);
		constexpr String(const String& other, MemoryPool& pool);
		constexpr String(String&& other) noexcept;
		constexpr String(String&& other, MemoryPool& pool);
		virtual ~String();
		String& operator=(const String& str);
		String& operator=(String&& str);
		String& operator=(const char* s);
		String& operator=(char ch);
		constexpr const MemoryPool& GetMemoryPool() const;

		// Element access
		constexpr char& operator[](size_t pos);
		constexpr const char& operator[](size_t pos) const;
		constexpr char& GetFront();
		constexpr const char& GetFront() const;
		constexpr char& GetBack();
		constexpr const char& GetBack() const;
		const char* GetCString() const;

		// Capacity
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;
		constexpr size_t GetLength() const noexcept;
		constexpr size_t GetMaxSize() const noexcept;
		constexpr void SetCapacity(size_t newCapacity);
		constexpr size_t GetCapacity() const noexcept;
		constexpr void Shrink();

		// Operations
		constexpr void Clear() noexcept;
		constexpr bool InsertAt(size_t index, size_t count, char ch);
		constexpr bool InsertAt(size_t index, const char* s);
		constexpr bool InsertAt(size_t index, const char* s, size_t count);
		constexpr bool InsertAt(size_t index, const String& str);
		constexpr bool InsertAt(size_t index, const String& str, size_t indexStr, size_t count = NPOS);
		constexpr bool RemoveAt(size_t index = 0, size_t count = NPOS);
		constexpr void PushBack(char ch);
		constexpr void PopBack();
		constexpr void Append(size_t count, char ch);
		constexpr void Append(const String& str);
		constexpr void Append(const String& str, size_t pos, size_t count = NPOS);
		constexpr void Append(const char* s, size_t count);
		constexpr void Append(const char* s);
		String& operator+=(const String& str);
		String& operator+=(char ch);
		String& operator+=(const char* s);
		constexpr bool StartsWith(const String& str) const noexcept;
		constexpr bool StartsWith(const String&& str) const noexcept;
		constexpr bool StartsWith(char c) const noexcept;
		constexpr bool StartsWith(const char* s) const;
		constexpr bool EndsWith(const String& str) const noexcept;
		constexpr bool EndsWith(const String&& str) const noexcept;
		constexpr bool EndsWith(char c) const noexcept;
		constexpr bool EndsWith(const char* s) const;
		constexpr bool Contains(const String& str) const noexcept;
		constexpr bool Contains(const String&& str) const noexcept;
		constexpr bool Contains(char c) const noexcept;
		constexpr bool Contains(const char* s) const;
		constexpr String& Replace(size_t pos, size_t count, const String& str);
		constexpr String& Replace(size_t pos, size_t count, const String& str, size_t pos2, size_t count2 = NPOS);
		constexpr String& Replace(size_t pos, size_t count, const char* cStr, size_t count2);
		constexpr String& Replace(size_t pos, size_t count, const char* cStr);
		constexpr String& Replace(size_t pos, size_t count, size_t count2, char ch);
		String GetSubstring(size_t pos = 0ul, size_t count = NPOS) const;
		constexpr void Resize(size_t count);
		constexpr void Resize(size_t count, char ch);

		// Search
		constexpr size_t GetIndexOf(const String& str, size_t pos = 0ul) const noexcept;
		constexpr size_t GetIndexOf(const char* s, size_t pos, size_t count) const;
		constexpr size_t GetIndexOf(const char* s, size_t pos = 0ul) const;
		constexpr size_t GetIndexOf(char ch, size_t pos = 0ul) const;
		constexpr size_t GetLastIndexOf(const String& str, size_t pos = NPOS) const noexcept;
		constexpr size_t GetLastIndexOf(const char* s, size_t pos, size_t count) const;
		constexpr size_t GetLastIndexOf(const char* s, size_t pos = NPOS) const;
		constexpr size_t GetLastIndexOf(char ch, size_t pos = NPOS) const;

		// Constants
		static constexpr size_t NPOS = -1;
		static constexpr size_t ALIGNED_BYTE = 16ul;
		static constexpr size_t MAX_SIZE = 1024ul;
	private:
		MemoryPool* mPool = &gCoreMemoryPool;
		size_t mLength = 0ul;
		size_t mCapacity = 0ul;
		char* mString = nullptr;
	};

	String ToString(int32_t value);
	String ToString(int64_t value);
	String ToString(uint32_t value);
	String ToString(uint64_t value);
	String ToString(float value);
	String ToString(double value);
	String ToString(long double value);

	String operator""_s(const char* str, size_t len);

	constexpr eResult Strcpy(char* dest, size_t destSize, const char* src, size_t count);
	constexpr eResult Strcat(char* dest, size_t destSize, const char* src, size_t count);
	constexpr size_t Strlen(const char* str, size_t strSize = String::MAX_SIZE);
	constexpr int32_t Strcmp(const char* lhs, const char* rhs, size_t count);

	constexpr eResult Strcpy(char* dest, size_t destSize, const char* src, size_t count)
	{
		if (dest == nullptr || src == nullptr)
		{
			return eResult::CAVE_INVALID_ARG;
		}
#ifdef __WIN32__
		if (strncpy_s(dest, destSize, src, count) != 0)
		{
			return eResult::CAVE_FAIL;
		}
#else

#ifdef CAVE_BUILD_DEBUG
		assert(Strlen(dest) <= destSize);
#endif
		if (destSize < count)
		{
			count = destSize;
		}
		
		if (strncpy(dest, src, count) != 0)
		{
			return eResult::CAVE_FAIL;
		}
#endif

		return eResult::CAVE_OK;
	}

	constexpr eResult Strcat(char* dest, size_t destSize, const char* src, size_t count)
	{
		if (dest == nullptr || src == nullptr)
		{
			return eResult::CAVE_INVALID_ARG;
		}
#ifdef __WIN32__
		if (strncat_s(dest, destSize, src, count) != 0)
		{
			return eResult::CAVE_FAIL;
		}
#else

#ifdef CAVE_BUILD_DEBUG
		assert(Strlen(dest) <= destSize);
#endif
		if (destSize < count)
		{
			count = destSize;
		}
		
		if (strncat(dest, src, count) != 0)
		{
			return eResult::CAVE_FAIL;
		}
#endif

		return eResult::CAVE_OK;
	}

	constexpr size_t Strlen(const char* str, size_t strSize)
	{
#ifdef __WIN32__
		return strlen_s(str, strSize);
#else
		if (str == nullptr)
		{
			return 0;
		}

		size_t size = 0ul;
		while (str[size++] != '\0' && size < strSize)
		{
		}

		return size;
#endif
	}

	constexpr int32_t Strcmp(const char* lhs, const char* rhs, size_t count)
	{
		return strncmp(lhs, rhs, count);
	}

#ifdef CAVE_BUILD_DEBUG
	namespace StringTest
	{
		void Test();
		void Constructor();
		void AssignmentOperator();
		void SubscriptOperator();
		void GetFront();
		void GetBack();
		void GetCString();
		void IsEmpty();
		void GetSize();
		void SetCapacity();
		void GetCapacity();
		void Shrink();
		void Clear();
		void InsertAt();
		void RemoveAt();
		void Append();
		void AdditionCompoundAssignmentOperator();
		void StartsWith();
		void EndsWith();
		void Contains();
		void Replace();
		void GetSubstring();
		void Resize();
		void GetIndexOf();
		void GetLastIndexOf();
		void AdditionOperator();
		void ComparisonOperator();
		void StreamOperator();
		void GetLine();
		void StringToInt();
		void StringToFloat();
		void ToString();
	}
#endif
} // namespace cave
#endif