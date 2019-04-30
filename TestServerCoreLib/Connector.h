#pragma once
#include "stdafx.h"
#include "Session.h"
#include "Receiver.h"
#include <string>


class CConnector : 
	public CSession
{
public:
	struct SInfo
	{
		std::string ConnectionAddr;
		__int16 PortNumber;
		__int32 ReceiverThreadCount;
	};

public:
	CConnector();
	~CConnector();

private:
	SInfo     m_connectionInfo;
	SOCKET    m_Socket;
	CReceiver m_Receiver;

public:

	/// 등록되어 있는 커넥션 정보를 가지고 접속 및 recv, send 등을 시작한다
	bool Start(SInfo const& init_);

	/// 커넥션을 닫고 관련 동작을 종료한다
	void Close();

	/// 해당 커넥션의 리시버 획득
	inline CReceiver& GetRecieverRef() { return m_Receiver; }

	/// 해당 커넥션의 세션을 가지고 온다
	inline SOCKET const GetSocket() { return m_Socket; }


	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;

};
