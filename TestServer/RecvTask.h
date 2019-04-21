#pragma once
#include "TaskBase.h"
#include "PoolObject.h"
#include "Packet.h"
#include "Session.h"

struct SRecvTask :
	public CTaskBase,
	public CPoolObject
{
	struct SInfo
	{
		CSession::INDEX   Index;
		packets::PacketID PacketId;
		__int32 Length;
		void* pBuffer;
	};

	CSession::INDEX    Index;
	packets::PacketID  PacketId;
	__int32            Length;
	void*              pBuffer;

	SRecvTask();
	~SRecvTask();

	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;
};