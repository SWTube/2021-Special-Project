/*!
 * Copyright (c) 2021 SWTube. All rights reserved.
 * Licensed under the GPL-3.0 License. See LICENSE file in the project root for license information.
 */
#include <cassert>
#include <iostream>

#include "Object/Obejct.h"
#include "Object/ObjectPool.h"
#include "Tmp/Log.h"

namespace cave
{
	Object::Object()
	{
		Log("Object::Object()");
	}

	Object::Object(const Object& other)
	{
		Log("Object::Object(const Object&)");
	}

	Object::Object(Object&& other) noexcept
	{
		Log("Object::Object(Object&&)");

		mOwner = other.mOwner;

		mHideFlags = other.mHideFlags;
		mName = std::move(other.mName);
		mTag = std::move(other.mTag);
		mTransform = std::move(other.mTransform);
		mInstanceID = other.mInstanceID;

		other.mOwner = nullptr;

		other.mHideFlags = 0x00000000;
		other.mInstanceID = 0;
	}

	Object::~Object()
	{
		Log("Object::~Object()");
	}

	Object& Object::operator=(const Object& other)
	{
		Log("Object::operator=(const Object&)");

		return *this;
	}

	Object& Object::operator=(Object&& other) noexcept
	{
		Log("Object::operator=(Object&&)");
		
		mOwner = other.mOwner;

		mHideFlags = other.mHideFlags;
		mName = std::move(other.mName);
		mTag = std::move(other.mTag);
		mTransform = std::move(other.mTransform);
		mInstanceID = other.mInstanceID;

		other.mOwner = nullptr;

		other.mHideFlags = 0x00000000;
		other.mInstanceID = 0;

		return *this;
	}

	void Object::Print()
	{
		Log("Object::Print()");

#ifdef _DEBUG
		assert(mOwner != nullptr);
		assert(mName != nullptr);
		assert(mTag != nullptr);
		std::cout << "===========================================================\n";
		std::cout << "Object[" << mInternalIndex << "]'s member\n\n";
		std::cout << "mOwner: " << mOwner << "\n";
		std::cout << "mName: " << mName << "\n";
		std::cout << "mTag: " << mTag << "\n";
		std::cout << "mHideFlags: " << mHideFlags << "\n";
		std::cout << "mInternalIndex: " << mInternalIndex << "\n";
		std::cout << "mInstanceID: " << mInstanceID << '\n';
#endif // _DEBUG

	}

	void Object::Destroy(Object& target, float timeDelay)
	{
		Log("Object::Destroy(Object&, float)");

		if (timeDelay == 0.f)
		{
			target.Initialize();
		}
	}

	void Object::DestroyImmediate(Object& target)
	{
		Log("Object::DestroyImmediate(Object&)");

		target.Initialize();
	}

	void Object::DontDestroyOnLoad(Object& target)
	{
		Log("Object::DontDestroyOnLoad");
	}

	Object& Object::FindObjectOfType(Type& type)
	{
		Log("Object::FindObjectOfType(Type&)");

		Object vec;

		return vec;
	}

	Object& Object::FindObjectsOfType(Type& type, bool includeInactive)
	{
		Log("Object::FindObjectsOfType(Type&, bool)");

		Object result;

		return result;
	}

	Object& Object::Instantiate(Object& original)
	{
		Log("Object::Instantiate(Object&)");

		Object tmp;/* = ObjectManager::Allocate();*/
		tmp.mName = original.mName;
		tmp.mHideFlags = original.mHideFlags;

		return tmp;
	}

	Object& Object::Instantiate(Object& original, Transform& parent)
	{
		Log("Object::Instantiate(Object&, Transform&)");

		Object tmp/* = ObjectManager::Allocate()*/;
		tmp.mName = original.mName;
		tmp.mHideFlags = original.mHideFlags;

		return tmp;
	}

	Object& Object::Instantiate(Object& original, Vector3& position, Quaternion& rotation)
	{
		Log("Object::Instantiate(Object&, Vector3&, Quaternion&)");

		Object tmp/* = ObjectManager::Allocate()*/;
		tmp.mName = original.mName;
		tmp.mHideFlags = original.mHideFlags;

		return tmp;
	}

	Object& Object::Instantiate(Object& original, Vector3& position, Quaternion& rotation, Transform& parent)
	{
		Log("Object::Instantiate(Object&, Vector3&, Quaternion&, Transform&)");

		Object tmp/* = ObjectManager::Allocate()*/;
		tmp.mName = original.mName;
		tmp.mHideFlags = original.mHideFlags;

		return tmp;
	}

	void Object::Initialize()
	{
		Log("Object::Initialize()");
		mHideFlags = 0x00000000;
		mInternalIndex = 0;
		mInstanceID = 0;
		mName = "";
		mTag = "";

		mTransform = { .x = 0.f, .y = 0.f};
	}

	void Object::SetOwner(Object& owner)
	{
		mOwner = &owner;
	}

	void Object::SetInternalIndex(unsigned int internalIndex)
	{
		Log("Object::SetInternalIndex(int)");

		mInternalIndex = internalIndex;
	}

	unsigned int Object::GetInternalIndex() const
	{
		Log("Object::GetInternalIndex()");

		return mInternalIndex;
	}

	unsigned int Object::GetInstanceID()
	{
		Log("Object::GetInstanceID()");

		return mInstanceID;
	}

	void Object::SetInstanceID(unsigned int id)
	{
		Log("Object::SetInstanceID(unsigned int)");

		mInstanceID = id;
	}

	const char* Object::ToString()
	{
		Log("Object::ToString()");

		return mName;
	}

	bool Object::CompareTag(const char* tag)
	{
		return strcmp(mTag, tag);
	}

	void Swap(Object& left, Object& right)
	{
		Log("cave::Swap(Object&, Object&)");

		Object tmp = std::move(left);
		left = std::move(right);
		right = std::move(tmp);
	}
}