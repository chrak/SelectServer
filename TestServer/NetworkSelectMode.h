#pragma once
#include "NetworkMode.h"
#include <winsock2.h>

class CSession;
class CNetworkSelectMode :
	public CNetworkMode
{
private:
	fd_set  m_ReadFd;
	timeval m_TimeOut;

public:
	CNetworkSelectMode();
	~CNetworkSelectMode();

public:
	virtual void Init(SOCKET socket_, SConfigInfo const& config_) override;
	virtual bool TransferProcess(CServerBase* server_) override;
	virtual bool LoginProcess(CServerBase* server_) override;
	

	

	bool RecvProcess(CSession* session_);
	;

	void CloseProcess(CSession* session_);
};