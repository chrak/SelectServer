#pragma once
#include "stdafx.h"
#include "MemoryPool.h"

template<typename T>
CMemoryPool<T>::CMemoryPool(const __int32 length_, const __int32 interval_)
	: m_Interval(interval_)
{
	if (length_ != AllocateObjects(length_))
	{
		//로그
	}
}

template<typename T>
CMemoryPool<T>::~CMemoryPool()
{
	ReleasePool();
}


template<typename T>
bool CMemoryPool<T>::AcquirePoolObject(T*& out_, void* param_)
{
	CPoolObject* obj;
	if (m_Queue.empty() || !m_Queue.try_pop(obj))
	{
		if (AllocateObjects(m_Interval) > 0)
		{
			if (!m_Queue.try_pop(obj))
			{
				// 로그
				return false;
			}
		}
		else
		{
			// 로그
			return false;
		}
	}

	if (!obj->Acquire(param_))
	{
		return false;
	}

	out_ = static_cast<T*>(obj);

	return true;
}

template<typename T>
bool CMemoryPool<T>::RetrievePoolObject(T* in_)
{
	CPoolObject* obj = static_cast<CPoolObject*>(in_);
	if (!obj->Retrieve())
	{
		return false;
	}
	
	m_Queue.push(obj);

	return true;
}

template<typename T>
__int32 CMemoryPool<T>::AllocateObjects(const __int32 length_)
{
	for (int i = 0; i < length_; ++i)
	{
		// 템플릿을 쓰지 않으면 타입의 크기를 알기 어렵다..(물론 편법이 있긴 하지만)
		T* obj = ALLOCATE_MALLOC(T, sizeof(T));
		if (obj == NULL)
		{
			//로그
			return i;
		}
		new(obj) T();
		m_Queue.push(obj);
		++m_TotalPoolSize;
	}

	return length_;
}

template<typename T>
void CMemoryPool<T>::ReleasePool()
{
	while (!m_Queue.empty())
	{
		CPoolObject* obj;
		m_Queue.try_pop(obj);

		obj->~CPoolObject();
		SAFE_FREE(obj);

	}
}
