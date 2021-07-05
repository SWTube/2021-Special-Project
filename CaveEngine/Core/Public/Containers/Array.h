/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#pragma once

#if 0
// temporary wrapper
#include "CoreTypes.h"
// include assertion macros
// include memory
#include "Memory/Allocator.h"
// include template
#include "Template/EnableIf.h"
#include "Template/IsIterator.h"
#include <type_traits>

#define GET_ARRAY_LENGTH(ARR) (sizeof(ARR) / sizeof(ARR[0]))

namespace cave
{
    template<size_t N>
    class TFloatArray
    {
    public:
        constexpr TFloatArray() = default;
        constexpr TFloatArray(const TFloatArray& other) = default;
        constexpr TFloatArray(TFloatArray&& other) = default;

        TFloatArray& operator=(const TFloatArray& other) = delete;
        TFloatArray& operator=(TFloatArray&& other) = delete;

        virtual ~TFloatArray() = default;

        constexpr size_t GetArrayLength() const
        {
            return N;
        }

        constexpr float& operator[](size_t index)
        {
            return mData[index];
        }

        constexpr float& operator*()
        {
            return *mData;
        }

        constexpr float* operator+(size_t index)
        {
            return mData + index;
        }
    private:
        float mData[N];
    };

    class FFloatArray
    {
        constexpr FFloatArray() = delete;
        constexpr explicit FFloatArray(size_t size);
        constexpr FFloatArray(const FFloatArray& other);
        constexpr FFloatArray(FFloatArray&& other) noexcept;

        FFloatArray& operator=(const FFloatArray& other) = delete;
        FFloatArray& operator=(FFloatArray&& other) = delete;

        virtual ~FFloatArray();

        constexpr size_t GetArrayLength() const;

        constexpr float& operator[](size_t index);

        constexpr float& operator*();

        constexpr float* operator+(size_t index);

    private:
        float* mData;
        size_t mSize;
    };

    template<typename ContainerType, typename ElementType, typename SizeType>
    class TArrayIterator
    {
    public:
        template<typename otherElementType, typename otherAllocatorType = TAllocator>
        friend class TArray;

        TArrayIterator(ContainerType& container, SizeType startIndex = 0)
            : mContainer(container), mIndex(startIndex)
        {
        }

        TArrayIterator& operator++()
        {
            ++mIndex;
            return *this;
        }

        TArrayIterator operator++(int)
        {
            TArrayIterator temp(*this);
            ++mIndex;
            return temp;
        }

        TArrayIterator& operator--()
        {
            --mIndex;
            return *this;
        }

        TArrayIterator operator--(int)
        {
            TArrayIterator temp(*this);
            --mIndex;
            return temp;
        }

        TArrayIterator& operator+=(SizeType offset)
        {
            mIndex += offset;
            return *this;
        }

        TArrayIterator operator+(SizeType offset)
        {
            TArrayIterator temp(*this);
            return temp += offset;
        }

        TArrayIterator& operator-=(SizeType offset)
        {
            mIndex -= offset;
            return *this;
        }

        TArrayIterator operator-(SizeType offset)
        {
            TArrayIterator temp(*this);
            return temp -= offset;
        }

        ElementType& operator*() const
        {
            return mContainer[mIndex];
        }

        ElementType* operator->() const
        {
            return &mContainer[mIndex];
        }

        SizeType GetIndex() const
        {
            return mIndex;
        }

        void Reset()
        {
            mIndex = 0;
        }

        friend bool operator==(const TArrayIterator& lhs, const TArrayIterator& rhs)
        {
            return &lhs.mContainer == &rhs.mContainer && lhs.mIndex == rhs.mIndex;
        }

        friend bool operator!=(const TArrayIterator& lhs, const TArrayIterator& rhs)
        {
            return &lhs.mContainer != &rhs.mContainer && lhs.mIndex == rhs.mIndex;
        }

    private:
        ContainerType& mContainer;
        SizeType mIndex;
    };

    template<typename ElementType, typename AllocatorType = TAllocator<ElementType>>
    class TArray
    {
    public:
        using Iterator = TArrayIterator<TArray, ElementType, size_t>;
        using ConstIterator = TArrayIterator<TArray, const ElementType, size_t>;

        constexpr explicit TArray(const AllocatorType& alloc) noexcept
            : mData(nullptr)
            , mMaxSize(0)
            , mCurrentSize(0)
            , mAllocator(alloc)
        {
        }

        constexpr TArray(size_t count, const ElementType& value, const AllocatorType& alloc = AllocatorType())
            : mAllocator(alloc)
        {
            mData = new ElementType[count];
            mMaxSize = count;
            mCurrentSize = count;
            for (size_t i = 0; i < count; ++i)
            {
                *(mData + i) = value;
            }
        }

        constexpr explicit TArray(size_t count, const AllocatorType& alloc = AllocatorType())
            : mAllocator(alloc)
        {
            mData = new ElementType[count];
            mMaxSize = count;
            mCurrentSize = 0;
        }

        template<typename InputIt, typename TEnableIf<TIsIterator<Iterator>::Value, bool>::Type = true>
        constexpr TArray(InputIt first, InputIt last, const AllocatorType& alloc = AllocatorType())
            : mAllocator(alloc)
        {
            size_t count = 0;
            for (InputIt iterator = first; iterator != last; ++iterator)
            {
                ++count;
            }
            mData = new ElementType[count];;
            mMaxSize = count;
            mCurrentSize = count;
            size_t i = 0;
            for (InputIt iterator = first; iterator != last; ++iterator, ++i)
            {
                *(mData + i) = *iterator;
            }
        }

        constexpr TArray(const TArray& other)
        {
            if (this != &other)
            {
                mAllocator = other.mAllocator;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = new ElementType[mMaxSize];
                for (size_t i = 0; i < mCurrentSize; ++i)
                {
                    mData[i] = other.mData[i];
                }
            }
            else
            {
                mAllocator = AllocatorType();
                mMaxSize = 0;
                mCurrentSize = 0;
                mData = nullptr;
            }
        }

        constexpr TArray(const TArray& other, const AllocatorType& alloc)
        {
            if (this != &other)
            {
                mAllocator = alloc;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = new ElementType[mMaxSize];
                for (size_t i = 0; i < mCurrentSize; ++i)
                {
                    mData[i] = other.mData[i];
                }
            }
            else
            {
                mAllocator = alloc;
                mMaxSize = 0;
                mCurrentSize = 0;
                mData = nullptr;
            }
        }

        constexpr TArray(TArray&& other) noexcept
        {
            if (this != &other)
            {
                mAllocator = other.mAllocator;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = other.mData;
                other.mMaxSize = 0;
                other.mCurrentSize = 0;
                other.mData = nullptr;
            }
            else
            {
                mAllocator = AllocatorType();
                mMaxSize = 0;
                mCurrentSize = 0;
                mData = nullptr;
            }
        }

        constexpr TArray(TArray&& other, const AllocatorType& alloc)
        {
            if (this != &other)
            {
                mAllocator = alloc;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = other.mData;
                other.mMaxSize = 0;
                other.mCurrentSize = 0;
                other.mData = nullptr;
            }
            else
            {
                mAllocator = alloc;
                mMaxSize = 0;
                mCurrentSize = 0;
                mData = nullptr;
            }
        }

        ~TArray()
        {
            if (mData != nullptr)
            {
                delete[] mData;
            }
        }

        constexpr TArray& operator=(const TArray& other)
        {
            if (this != &other)
            {
                mAllocator = other.mAllocator;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = new ElementType[mMaxSize];
                for (size_t i = 0; i < mCurrentSize; ++i)
                {
                    mData[i] = other.mData[i];
                }
            }

            return *this;
        }

        constexpr TArray& operator=(TArray&& other) noexcept(std::allocator_traits<AllocatorType>::propagate_on_container_move_assignment::value || std::allocator_traits<AllocatorType>::is_always_equal::value)
        {
            if (this != &other)
            {
                mAllocator = other.mAllocator;
                mMaxSize = other.mMaxSize;
                mCurrentSize = other.mCurrentSize;
                mData = other.mData;
                other.mMaxSize = 0;
                other.mCurrentSize = 0;
                other.mData = nullptr;
            }

            return *this;
        }

        constexpr void Reserve(size_t maxSize)
        {
            if (maxSize > mMaxSize)
            {
                mMaxSize = maxSize;
                ElementType* temp = new ElementType[mMaxSize];

                for (int i = 0; i < mCurrentSize; ++i)
                {
                    temp[i] = mData[i];
                }

                delete[] mData;
                mData = temp;
            }
        }
        
        template<typename = typename TEnableIf<std::is_constructible_v<ElementType>>::Type>
        constexpr void Resize(size_t size)
        {
            if (size > mMaxSize)
            {
                mMaxSize = size;
                ElementType* temp = new ElementType[mMaxSize];

                for (int i = 0; i < mCurrentSize; ++i)
                {
                    temp[i] = mData[i];
                }
                for (int i = mCurrentSize; i < mMaxSize; ++i)
                {
                    temp[i] = ElementType();
                }

                delete[] mData;
                mData = temp;
            }
            else if (size > mCurrentSize)
            {
                for (int i = mCurrentSize; i < size; ++i)
                {
                    mData[i] = ElementType();
                }
                mCurrentSize = size;
            }
            else
            {
                mCurrentSize = size;
            }
        }

        constexpr void Resize(size_t size, const ElementType& initializeElement)
        {
            if (size > mMaxSize)
            {
                mMaxSize = size;
                ElementType* temp = new ElementType[mMaxSize];

                for (int i = 0; i < mCurrentSize; ++i)
                {
                    temp[i] = mData[i];
                }
                for (int i = mCurrentSize; i < mMaxSize; ++i)
                {
                    temp[i] = initializeElement;
                }

                delete[] mData;
                mData = temp;
            }
            else if(size > mCurrentSize)
            {
                for (int i = mCurrentSize; i < size; ++i)
                {
                    mData[i] = initializeElement;
                }
                mCurrentSize = size;
            }
            else
            {
                mCurrentSize = size;
            }
        }

        constexpr void ClearElements()
        {
            mCurrentSize = 0;
        }

        constexpr void Assign(size_t count, const ElementType& initializeElement)
        {
            ClearElements();
            Resize(count, initializeElement);
        }

        template<class InputIt, typename TEnableIf<TIsIterator<Iterator>::Value, bool>::Type = true>
        constexpr void Assign(InputIt first, InputIt last)
        {
            int count = 0;

            for (InputIt iterator = first; iterator != last; ++iterator, ++count);
            ClearElements();
            Reserve(count);

            int i = 0;
            for (InputIt iterator = first; iterator != last; ++iterator, ++i)
            {
                mData[i] = *iterator;
            }
        }

        constexpr Iterator Insert(Iterator position, const ElementType& element)
        {
            size_t index = position.mIndex;
            size_t newMaxSize = mMaxSize;

            if (mCurrentSize + 1 > newMaxSize)
            {
                newMaxSize *= 2;
            }
            Reserve(newMaxSize);

            for (size_t i = mCurrentSize - 1; i > index - 1; --index)
            {
                mData[i + 1] = mData[i];
            }

            mCurrentSize += 1;
            mData[index] = element;

            return Iterator(*this, index);
        }

        constexpr Iterator Insert(Iterator position, size_t count, const ElementType& element)
        {
            size_t index = position.mIndex;
            size_t newMaxSize = mMaxSize;

            while (mCurrentSize + count > newMaxSize)
            {
                newMaxSize *= 2;
            }
            Reserve(newMaxSize);

            for (size_t i = mCurrentSize - 1; i > index - 1; --index)
            {
                mData[i + count] = mData[i];
            }

            mCurrentSize += count;

            for (size_t i = 0; i < count; ++i)
            {
                mData[index + i] = element;
            }

            return Iterator(*this, index);
        }

        template<class InputIt, typename TEnableIf<TIsIterator<Iterator>::Value, bool>::Type = true>
        constexpr Iterator Insert(Iterator position, InputIt first, InputIt last)
        {
            size_t count = 0;
            size_t index = position.mIndex;
            size_t newMaxSize = mMaxSize;

            for (InputIt iterator = first; iterator != last; ++iterator)
            {
                ++count;
            }

            while (mCurrentSize + count > newMaxSize)
            {
                newMaxSize *= 2;
            }
            Reserve(newMaxSize);

            for (size_t i = mCurrentSize - 1; i > index - 1; --index)
            {
                mData[i + count] = mData[i];
            }

            mCurrentSize += count;

            size_t i = 0;
            for (InputIt iterator = first; iterator != last; ++iterator, ++i)
            {
                mData[index + i] = *iterator;
            }

            return Iterator(*this, index);
        }

        constexpr Iterator Erase(Iterator position)
        {
            size_t index = position.mIndex;

            for (size_t i = index; i < mCurrentSize; ++i)
            {
                mData[i] = mData[i + 1];
            }

            mCurrentSize -= 1;

            return Iterator(*this, index);
        }

        constexpr Iterator Erase(Iterator first, Iterator last)
        {
            size_t count = 0;
            size_t index = first.mIndex;

            for (Iterator iterator = first; iterator != last; ++iterator)
            {
                ++count;
            }

            for (size_t i = index; i < mCurrentSize; ++i)
            {
                mData[i] = mData[i + count];
            }

            mCurrentSize -= count;

            return Iterator(*this, index);
        }

        template<typename... Args>
        constexpr Iterator Emplace(Iterator position, Args&&... args)
        {
            size_t index = position.mIndex;
            size_t newMaxSize = mMaxSize;

            if (mCurrentSize + 1 > newMaxSize)
            {
                newMaxSize *= 2;
            }
            Reserve(newMaxSize);

            for (size_t i = mCurrentSize - 1; i > index - 1; --index)
            {
                mData[i + 1] = mData[i];
            }

            mCurrentSize += 1;
            mData[index] = ElementType(args...);

            return Iterator(*this, index);
        }

        constexpr void PushBack(const ElementType& element)
        {
            Insert(GetEndIterator(), element);
        }

        constexpr void EmplaceBack(const ElementType& element)
        {
            Emplace(GetEndIterator(), element);
        }

        constexpr void PopBack()
        {
            --mCurrentSize;
        }

        constexpr bool IsEmpty() const
        {
            return (mCurrentSize == 0);
        }

        constexpr size_t GetSize() const
        {
            return mCurrentSize;
        }

        constexpr size_t GetMaxSize() const
        {
            return mMaxSize;
        }

        constexpr AllocatorType GetAllocator() const noexcept
        {
            return mAllocator;
        }

        // Element Access
        constexpr ElementType& operator[](size_t pos)
        {
            return *(mData + pos);
        }

        constexpr const ElementType& operator[](size_t pos) const
        {
            return *(mData + pos);
        }

        constexpr ElementType& GetFront()
        {
            return *mData;
        }

        constexpr const ElementType& GetFront() const
        {
            return *mData;
        }

        constexpr ElementType& GetBack()
        {
            return *(mData + mCurrentSize - 1);
        }

        constexpr const ElementType& GetBack() const
        {
            return *(mData + mCurrentSize - 1);
        }

        constexpr ElementType* GetData() noexcept
        {
            return mData;
        }

        constexpr const ElementType* GetData() const noexcept
        {
            return mData;
        }

        // Iterators

        constexpr Iterator GetBeginIterator()
        {
            return Iterator(*this, 0);
        }

        constexpr Iterator GetEndIterator()
        {
            return Iterator(*this, 0 + mCurrentSize);
        }

        constexpr ConstIterator GetBeginConstIterator() const
        {
            return ConstIterator(*this, 0);
        }

        constexpr ConstIterator GetEndConstIterator() const
        {
            return ConstIterator(*this, 0 + mCurrentSize);
        }

    private:
        size_t mCurrentSize;
        size_t mMaxSize;
        ElementType* mData;
        AllocatorType mAllocator;
    };
} // namespace cave
#endif