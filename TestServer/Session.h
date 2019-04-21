#pragma once

#include <winsock2.h>
#include <tbb/concurrent_queue.h>
#include "CommonDef.h"
#include "PoolObject.h"


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

		SSendInfo() :
			pBuf(NULL)
		{
		}
	};

	struct SRecvInfo
	{
		char* pBuf;
		__int32 prevRecvPos;
		__int32 remainLen;

		SRecvInfo() :
			pBuf(NULL),
			prevRecvPos(0),
			remainLen(0)
		{
		}
	};

private:
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

	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;
};