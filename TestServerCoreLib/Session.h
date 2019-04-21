#pragma once

#include <winsock2.h>
#include <tbb/concurrent_queue.h>
#include "CommonDef.h"
#include "PoolObject.h"
#include "PacketDef.h"
#include "flatbuffers/flatbuffers.h"


class CSession : 
	public CPoolObject
{
public:
	typedef __int32 INDEX;

	struct SInfo
	{
		SOCKET Socket;
		INDEX Index;
	};

	struct SSendInfo
	{
		char* pBuf;
		__int32 RemainLen;

		SSendInfo() :
			RemainLen(0),
			pBuf(NULL)
		{
		}
	};

	struct SRecvInfo
	{
		char* pBuf;
		__int32 PrevRecvPos;
		__int32 RemainLen;

		SRecvInfo() :
			pBuf(NULL),
			PrevRecvPos(0),
			RemainLen(0)
		{
		}
	};

protected:
	INDEX  m_Index;
	SOCKET m_Socket;
	SSendInfo m_sendInfo;
	SRecvInfo m_recvInfo;
	
	bool  m_IsConnected;
	char  m_IP[MAX_IP_LEN];

public:
	CSession();
	virtual ~CSession();

public:
	inline SOCKET GetSocket() { return m_Socket; }
	inline INDEX  GetIndex() { return m_Index; }
	inline SSendInfo& GetSendRef() { return m_sendInfo; }
	inline SRecvInfo& GetRecvRef() { return m_recvInfo; }
	inline bool IsConnected() const { return m_IsConnected; }

	bool SendPacket(packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_);

	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;
};