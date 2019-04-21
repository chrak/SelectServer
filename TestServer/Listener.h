#pragma once
#include <winsock2.h>
#include "NetworkMode.h"
#include "ThreadRegister.hpp"

class CNetworkMode;
class CServerBase;
class CListener : 
	public CTaskThreadRegister<CListener>
{
private:
	CServerBase* m_pServer;            ///< 서버
	SOCKET m_ListenSocket;             ///< Listen 소켓
	__int16 m_ListenPort;              ///< 리슨 포트
	
public:
	CListener(CServerBase* server_);
	~CListener();

public:
	bool Start(CNetworkMode::SConfigInfo const& config_);
	
	inline SOCKET GetSocket() { return m_ListenSocket; }

	void PushTask(CTaskBase* task_);

	virtual bool Process(CTaskBase* task_) override;
};
