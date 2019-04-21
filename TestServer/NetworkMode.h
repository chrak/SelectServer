#pragma once
#include <WinSock2.h>

class CServerBase;
class CTaskBase;
class CNetworkMode
{
public:
	struct SConfigInfo
	{
		__int16 PortNumber;
		__int32 TimeOutSec;
	};

public:
	CNetworkMode() {}
	~CNetworkMode() {}

public:
	virtual void Init(SOCKET socket_, SConfigInfo const& config_) = 0;
	virtual bool TransferProcess(CServerBase* server_) = 0;
	virtual bool LoginProcess(CServerBase* server_) = 0;
	//virtual bool RecvProcess(CServerBase* server_) = 0;
};