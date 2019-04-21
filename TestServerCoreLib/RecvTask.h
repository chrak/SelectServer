#pragma once
#include "TaskBase.h"
#include "PoolObject.h"
#include "PacketDef.h"
#include "Session.h"

struct SRecvTask :
	public CTaskBase
{
	struct SInfo
	{
		CSession::INDEX Index;
		packetdef::PacketID PacketId;
		__int32 Length;
		void* pBuffer;
	};

	CSession::INDEX     Index;
	packetdef::PacketID PacketId;
	__int32             Length;
	char                pBuffer[PACKET_SIZE];

	SRecvTask();
	~SRecvTask();

	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;
};