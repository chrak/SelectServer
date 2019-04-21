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
	CServerBase* m_pServer;            ///< ����
	SOCKET m_ListenSocket;             ///< Listen ����
	__int16 m_ListenPort;              ///< ���� ��Ʈ
	
public:
	CListener(CServerBase* server_);
	~CListener();

public:
	bool Start(CNetworkMode::SConfigInfo const& config_);
	
	inline SOCKET GetSocket() { return m_ListenSocket; }

	void PushTask(CTaskBase* task_);

	virtual bool Process(CTaskBase* task_) override;
};
