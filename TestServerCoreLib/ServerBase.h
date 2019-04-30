#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define FD_SETSIZE 1024

#include <winsock2.h>
#include <map>
#include "NetworkMode.h"
#include "ThreadRegister.hpp"
#include "Listener.h"
#include "Receiver.h"
#include "PacketDef.h"
#include "CommonDef.h"

class CMessageContextBase;

///////////////////////////////////////////////////////////////////////////
// ���� �⺻ class
///////////////////////////////////////////////////////////////////////////
class CServerBase :
	public CTaskThreadRegister<CServerBase>
{
protected:
	ServerType m_ServerType;
	CNetworkMode::SConfigInfo m_ServerInfo;           ///< ���� ���� ����ü

	SOCKADDR_IN m_ServerAddr;
	CListener m_Listener;
	CReceiver m_Receiver;
	CNetworkMode* m_pNetworkMode;       ///< ��Ʈ��ũ ���

	bool m_IsInit;                ///< �ʱ�ȭ �Ϸ� ����
	bool m_IsRunning;             ///< ������ ������ ����

public:
	/// ������ ���۷��� ȹ��
	inline CListener& GetListenerRef() { return m_Listener; }

	/// ���ù� ���۷��� ȹ��
	inline CReceiver& GetRecieverRef() { return m_Receiver; }

	/// ����Ÿ�� ȹ��
	inline ServerType GetServerType() { return m_ServerType; }

	/// �޽��� ���ؽ�Ʈ ���
	bool RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_);

	/// �ʱ�ȭ
	virtual void Init(CNetworkMode::SConfigInfo const& serverInfo_);

	/// Thread ��� �� ����
	virtual bool Start();

	/// ��Ʈ��ũ ��� ���̱�
	void AttachNetworkMode(CNetworkMode* mode_);

	/// ��Ʈ��ũ ��� ��������
	CNetworkMode* GetNetworkMode() const;

	/// serverTask ó��
	virtual bool Process(CTaskBase* task_);

	/// ���� ó��
	virtual void Close() override;


public:
	CServerBase();
	virtual ~CServerBase();
};