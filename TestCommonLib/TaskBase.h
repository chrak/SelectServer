#pragma once

#include <tbb/atomic.h>
#include "PoolObject.h"


class CTaskBase : 
	public CPoolObject
{
private:
	tbb::atomic<__int32> m_ExpireCount;    ///< 0�� �Ǹ� �����Ǵ� ī��Ʈ

public:
	CTaskBase();
	~CTaskBase();

public:
	/// �׽�ũ�� ������ ���� �ݵ�� �����ؾ� �ϴ� ��
	void SetExpireCount(__int32 count_) { m_ExpireCount = count_; }

	/// �����ؾ� �� �׽�ũ�� ����Ǿ��°��� Ȯ��
	inline bool CheckExepired() { return m_ExpireCount.fetch_and_decrement() <= 1; }

	/// ���� �Լ�
	virtual bool Run();

	/// Ǯ ����
	virtual bool Acquire(void*);
	
	/// Ǯ ����
	virtual bool Retrieve();

};