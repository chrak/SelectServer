#pragma once

#include <tbb/atomic.h>
#include "PoolObject.h"


class CTaskBase : 
	public CPoolObject
{
private:
	tbb::atomic<__int32> m_ExpireCount;    ///< 0이 되면 삭제되는 카운트

public:
	CTaskBase();
	~CTaskBase();

public:
	/// 테스크를 보내기 전에 반드시 세팅해야 하는 값
	void SetExpireCount(__int32 count_) { m_ExpireCount = count_; }

	/// 수행해야 할 테스크가 만료되었는가를 확인
	inline bool CheckExepired() { return m_ExpireCount.fetch_and_decrement() <= 1; }

	/// 수행 함수
	virtual bool Run();

	/// 풀 습득
	virtual bool Acquire(void*);
	
	/// 풀 복귀
	virtual bool Retrieve();

};