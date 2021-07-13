/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#pragma once

// temporary wrapper
#include "CoreTypes.h"
// include assertion macros
#include "Assertion/Assert.h"
// include memory
#include "CoreGlobals.h"
#include "Memory/Memory.h"
// include template
import IteratorType;

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
        template<typename otherElementType>
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
            return &lhs.mContainer != &rhs.mContainer || lhs.mIndex != rhs.mIndex;
        }

    private:
        ContainerType& mContainer;
        SizeType mIndex;
    };

    template<class ElementType>
    class TArray
    {
    public:
        using Iterator = TArrayIterator<TArray, ElementType, size_t>;
        using ConstIterator = TArrayIterator<TArray, const ElementType, size_t>;

        constexpr explicit TArray(MemoryPool& pool = gCoreMemoryPool) noexcept;
        constexpr TArray(size_t count, const ElementType& value, MemoryPool& pool = gCoreMemoryPool);
        constexpr TArray(size_t count, MemoryPool& pool = gCoreMemoryPool);
        template<IterType InputIt>
        constexpr TArray(InputIt first, InputIt last, MemoryPool& pool = gCoreMemoryPool);
        constexpr TArray(const TArray& other, MemoryPool& pool = gCoreMemoryPool);
        constexpr TArray(TArray&& other, MemoryPool& pool = gCoreMemoryPool) noexcept;
        ~TArray();

        constexpr TArray& operator=(const TArray& other);
        constexpr TArray& operator=(TArray&& other);


        constexpr void Reserve(size_t maxSize);

        constexpr void Resize(size_t size, const ElementType& initializeElement);

        constexpr void ClearElements();


        constexpr void Assign(size_t count, const ElementType& initializeElement);
        template<IterType Iter>
        constexpr void Assign(Iter first, Iter last);

        constexpr Iterator Insert(Iterator position, const ElementType& element);
        constexpr Iterator Insert(Iterator position, size_t count, const ElementType& element);
        template<IterType Iter>
        constexpr Iterator Insert(Iterator position, Iter first, Iter last);

        constexpr Iterator Erase(Iterator position);
        constexpr Iterator Erase(Iterator first, Iterator last);

        constexpr void PushBack(const ElementType& element);

        constexpr void PopBack();

        constexpr bool IsEmpty() const;

        constexpr size_t GetSize() const;

        constexpr size_t GetCapacity() const;

        constexpr size_t GetMaxSize() const;


        // Element Access
        constexpr ElementType& operator[](size_t pos);
        constexpr const ElementType& operator[](size_t pos) const;

        constexpr ElementType& GetFront();
        constexpr const ElementType& GetFront() const;

        constexpr ElementType& GetBack();
        constexpr const ElementType& GetBack() const;

        constexpr ElementType* GetData() noexcept;
        constexpr const ElementType* GetData() const noexcept;

        // Iterators
        constexpr Iterator GetBeginIterator();
        constexpr Iterator GetEndIterator();

        constexpr ConstIterator GetBeginConstIterator() const;
        constexpr ConstIterator GetEndConstIterator() const;

    private:
        MemoryPool* mPool;
        size_t mSize;
        size_t mCapacity;
        ElementType* mData;
    };

    template<class ElementType>
    constexpr TArray<ElementType>::TArray(MemoryPool& pool) noexcept
        : mPool(&pool)
        , mSize(0)
        , mCapacity(0)
        , mData(nullptr)
    { }

    template<class ElementType>
    constexpr TArray<ElementType>::TArray(size_t count, const ElementType& value, MemoryPool& pool)
        : mPool(&pool)
        , mSize(count)
        , mCapacity(mSize)
        , mData(reinterpret_cast<ElementType*>(mPool->Allocate(sizeof(ElementType)* mCapacity)))
    {
        for (size_t i = 0; i < count; ++i)
        {
            mData[i] = value;
        }
    }

    template<class ElementType>
    constexpr TArray<ElementType>::TArray(size_t count, MemoryPool& pool)
        : mPool(&pool)
        , mSize(0)
        , mCapacity(count)
        , mData(reinterpret_cast<ElementType*>(mPool->Allocate(sizeof(ElementType)* mCapacity)))
    { }

    template<class ElementType>
    template<IterType InputIt>
    constexpr TArray<ElementType>::TArray(InputIt first, InputIt last, MemoryPool& pool)
        : mPool(&pool)
    {
        size_t count = 0;
        //GetIteratorDIstance(first, last)
        for (InputIt iter = first; iter != last; ++iter, ++count);

        mSize = count;
        mCapacity = count;
        mData = reinterpret_cast<ElementType*>(mPool->Allocate(sizeof(ElementType) * count));
        new(mData) ElementType[count];

        size_t i = 0;
        for (InputIt iterator = first; iterator != last; ++iterator, ++i)
        {
            mData[i] = *iterator;
        }
    }

    template<class ElementType>
    constexpr TArray<ElementType>::TArray(const TArray& other, MemoryPool& pool)
        : mPool(&pool)
        , mSize(other.mSize)
        , mCapacity(other.mCapacity)
        , mData(nullptr)
    {
        if (mCapacity > 0)
        {
            mData = reinterpret_cast<ElementType*>(mPool->Allocate(sizeof(ElementType) * mCapacity));
            Memory::Memcpy(mData, other.mData, sizeof(ElementType) * mSize);
        }
    }

    template<class ElementType>
    constexpr TArray<ElementType>::TArray(TArray&& other, MemoryPool& pool) noexcept
        : mPool(&pool)
        , mSize(other.mSize)
        , mCapacity(other.mCapacity)
        , mData(other.mData)
    {
        other.mCapacity = 0;
        other.mSize = 0;
        other.mData = nullptr;
    }

    template<class ElementType>
    TArray<ElementType>::~TArray()
    {
        if (mData != nullptr)
        {
            mPool->Deallocate(mData, mCapacity * sizeof(ElementType));
        }
    }

    template<class ElementType>
    constexpr TArray<ElementType>& TArray<ElementType>::operator=(const TArray& other)
    {
        if (this != &other)
        {
            ClearElements();
            Reserve(other.mCapacity);
            mSize = other.mSize;

            Memory::Memcpy(mData, other.mData, sizeof(ElementType) * mSize);
        }

        return *this;
    }

    template<class ElementType>
    constexpr TArray<ElementType>& TArray<ElementType>::operator=(TArray&& other)
    {
        if (this != &other)
        {
            mCapacity = other.mCapacity;
            mSize = other.mSize;
            mData = other.mData;
            other.mCapacity = 0;
            other.mSize = 0;
            other.mData = nullptr;
        }

        return *this;
    }

    template<class ElementType>
    constexpr void TArray<ElementType>::Reserve(size_t maxSize)
    {
        assert(maxSize <= GetMaxSize());

        if (maxSize > mCapacity)
        {
            ElementType* temp = reinterpret_cast<ElementType*>(mPool->Allocate(sizeof(ElementType) * maxSize));

            if (mData != nullptr)
            {
                Memory::Memcpy(temp, mData, mSize * sizeof(ElementType));
                mPool->Deallocate(mData, mCapacity * sizeof(ElementType));
            }

            mData = temp;
            mCapacity = maxSize;
        }
    }
    
    template<class ElementType>
    constexpr void TArray<ElementType>::Resize(size_t size, const ElementType& initializeElement)
    {
        if (size > mCapacity)
        {
            Reserve(size);
        }

        if (size > mSize)
        {
            for (int i = mSize; i < size; ++i)
            {
                mData[i] = initializeElement;
            }
        }

        mSize = size;
    }

    template<class ElementType>
    constexpr void TArray<ElementType>::ClearElements()
    {
        mSize = 0;
    }

    template<class ElementType>
    constexpr void TArray<ElementType>::Assign(size_t count, const ElementType& initializeElement)
    {
        ClearElements();
        Resize(count, initializeElement);
    }

    template<class ElementType>
    template<IterType Iter>
    constexpr void TArray<ElementType>::Assign(Iter first, Iter last)
    {
        size_t count = 0;
        for (Iter iterator = first; iterator != last; ++iterator, ++count);

        ClearElements();
        Reserve(count);

        size_t i = 0;
        for (Iter iterator = first; iterator != last; ++iterator, ++i)
        {
            mData[i] = *iterator;
        }
        mSize = count;
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::Insert(Iterator position, const ElementType& element)
    {
        size_t index = position.mIndex;
        
        assert(index >= 0);
        assert(index <= mSize);

        size_t newMaxSize = mCapacity;

        if (newMaxSize == 0)
        {
            newMaxSize = 1;
        }
        if (mSize + 1 > newMaxSize)
        {
            newMaxSize <<= 1;
        }
        Reserve(newMaxSize);
        
        Memory::Memmove(mData + index + 1, mData + index, sizeof(ElementType) * (mSize - index));
        
        mSize += 1;
        mData[index] = element;

        return Iterator(*this, index);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::Insert(TArray<ElementType>::Iterator position, size_t count, const ElementType& element)
    {
        size_t index = position.mIndex;

        assert(index >= 0);
        assert(index <= mSize);

        size_t newMaxSize = mCapacity;

        if (newMaxSize == 0)
        {
            newMaxSize = 1;
        }
        while (mSize + count > newMaxSize)
        {
            newMaxSize <<= 1;
        }
        Reserve(newMaxSize);

        Memory::Memmove(mData + index + count, mData + index, sizeof(ElementType) * (mSize - index));

        mSize += count;

        for (size_t i = 0; i < count; ++i)
        {
            mData[index + i] = element;
        }

        return Iterator(*this, index);
    }

    template<class ElementType>
    template<IterType Iter>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::Insert(Iterator position, Iter first, Iter last)
    {
        size_t count = 0;
        size_t index = position.mIndex;

        assert(index >= 0);
        assert(index <= mSize);

        size_t newMaxSize = mCapacity;

        for (Iter iterator = first; iterator != last; ++iterator, ++count);

        if (newMaxSize == 0)
        {
            newMaxSize = 1;
        }
        while (mSize + count > newMaxSize)
        {
            newMaxSize *= 2;
        }
        Reserve(newMaxSize);

        Memory::Memmove(mData + index + count, mData + index, sizeof(ElementType) * (mSize - index));

        mSize += count;

        size_t i = 0;
        for (Iter iterator = first; iterator != last; ++iterator, ++i)
        {
            mData[index + i] = *iterator;
        }

        return Iterator(*this, index);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::Erase(Iterator position)
    {
        size_t index = position.mIndex;

        assert(index >= 0);
        assert(index < mSize);

        Memory::Memmove(mData + index, mData + index + 1, sizeof(ElementType) * (mSize - index - 1));
        

        mSize -= 1;

        return Iterator(*this, index);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::Erase(Iterator first, Iterator last)
    {
        size_t count = 0;
        size_t index = first.mIndex;

        for (Iterator iterator = first; iterator != last; ++iterator, ++count);

        Memory::Memmove(mData + index, mData + index + count, sizeof(ElementType) * (mSize - index - count));

        mSize -= count;

        return Iterator(*this, index);
    }

    template<class ElementType>
    constexpr void TArray<ElementType>::PushBack(const ElementType& element)
    {
        Insert(GetEndIterator(), element);
    }

    template<class ElementType>
    constexpr void TArray<ElementType>::PopBack()
    {
        --mSize;
    }

    template<class ElementType>
    constexpr bool TArray<ElementType>::IsEmpty() const
    {
        return (mSize == 0);
    }

    template<class ElementType>
    constexpr size_t TArray<ElementType>::GetSize() const
    {
        return mSize;
    }

    template<class ElementType>
    constexpr size_t TArray<ElementType>::GetCapacity() const
    {
        return mCapacity;
    }

    template<class ElementType>
    constexpr size_t TArray<ElementType>::GetMaxSize() const
    {
        return mPool->GetFreeMemorySize() / sizeof(ElementType);
    }

    template<class ElementType>
    constexpr ElementType& TArray<ElementType>::operator[](size_t pos)
    {
        return *(mData + pos);
    }

    template<class ElementType>
    constexpr const ElementType& TArray<ElementType>::operator[](size_t pos) const
    {
        return *(mData + pos);
    }

    template<class ElementType>
    constexpr ElementType& TArray<ElementType>::GetFront()
    {
        return *mData;
    }

    template<class ElementType>
    constexpr const ElementType& TArray<ElementType>::GetFront() const
    {
        return *mData;
    }

    template<class ElementType>
    constexpr ElementType& TArray<ElementType>::GetBack()
    {
        return *(mData + mSize - 1);
    }

    template<class ElementType>
    constexpr const ElementType& TArray<ElementType>::GetBack() const
    {
        return *(mData + mSize - 1);
    }

    template<class ElementType>
    constexpr ElementType* TArray<ElementType>::GetData() noexcept
    {
        return mData;
    }

    template<class ElementType>
    constexpr const ElementType* TArray<ElementType>::GetData() const noexcept
    {
        return mData;
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::GetBeginIterator()
    {
        return Iterator(*this, 0);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::Iterator TArray<ElementType>::GetEndIterator()
    {
        return Iterator(*this, 0 + mSize);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::ConstIterator TArray<ElementType>::GetBeginConstIterator() const
    {
        return ConstIterator(*this, 0);
    }

    template<class ElementType>
    constexpr typename TArray<ElementType>::ConstIterator TArray<ElementType>::GetEndConstIterator() const
    {
        return ConstIterator(*this, 0 + mSize);
    }

} // namespace cave