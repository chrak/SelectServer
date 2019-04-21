#pragma once
#include <tbb/concurrent_queue.h>
#include "PoolObject.h"

template<typename T>
class CMemoryPool
{
public:
	typedef tbb::concurrent_queue<CPoolObject*> OBJECT_POOL_QUEUE;

private:
	OBJECT_POOL_QUEUE m_Queue;
	__int32 m_Interval;
	__int32 m_TotalPoolSize;

public:
	bool AcquirePoolObject(T*& out_, void* param_);
	bool RetrievePoolObject(T* in_);
	__int32 AllocateObjects(const __int32 length_);
	void ReleasePool();

public:
	CMemoryPool(const __int32 length_, const __int32 interval_);
	virtual ~CMemoryPool();

};