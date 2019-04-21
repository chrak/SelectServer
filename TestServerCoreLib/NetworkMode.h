#pragma once
#include <WinSock2.h>
#include "CommonDef.h"

class CServerBase;
class CSession;
class CTaskBase;
class CNetworkMode
{
public:
	struct SConfigInfo
	{
		__int16 PortNumber;
		__int32 TimeOutSec;
		__int32 RecvThreadCount;
		__int16 BackLogSize;
		ServerType ServerType;
	};

public:
	CNetworkMode() {}
	~CNetworkMode() {}

public:
	virtual void Init(SOCKET const socket_, SConfigInfo const& config_) = 0;
	virtual bool TransferProcess(CServerBase* server_) = 0;
	virtual bool LoginProcess(CServerBase* server_) = 0;
	virtual bool RecvProcess(CSession* session_) = 0;
	virtual void CloseProcess(CSession* session_) = 0;
	virtual void SetSockOption(SOCKET const socket_) = 0;
};