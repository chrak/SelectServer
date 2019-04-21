#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define FD_SETSIZE 1024

#include <winsock2.h>
#include <map>
#include "NetworkMode.h"
#include "ServerTask.h"
#include "ThreadRegister.h"
#include "Listener.h"
#include "Receiver.h"
#include "Packet.h"

class CMessageContextBase;

///////////////////////////////////////////////////////////////////////////
// ���� �⺻ class
///////////////////////////////////////////////////////////////////////////
class CServerBase :
	public CTaskThreadRegister<CServerBase>
{
public:
	typedef std::map<packets::PacketRegion, CMessageContextBase*> CONTEXT_HANDLER_MAP;

protected:
	CNetworkMode::SConfigInfo m_ServerInfo;           ///< ���� ���� ����ü
	CONTEXT_HANDLER_MAP m_ContextMap;

	SOCKADDR_IN m_ServerAddr;
	CListener m_Listener;
	CReciever m_Receiver;

	CNetworkMode* m_pNetworkMode;       ///< ��Ʈ��ũ ���

	int server_sockfd, client_sockfd;
	int state, client_len;
	int client[FD_SETSIZE];


	bool m_IsInit;                ///< �ʱ�ȭ �Ϸ� ����
	bool m_IsRunning;             ///< ������ ������ ����

public:
	/// ������ ���۷��� ȹ��
	CListener& const GetListenerRef() { return m_Listener; }

	/// ���ù� ���۷��� ȹ��
	CReciever& const GetRecieverRef() { return m_Receiver; }

	/// �޽��� ���ؽ�Ʈ ���
	bool RegistContext(packets::PacketRegion const region_, CMessageContextBase* context_);

	/// serverTask Thread ��� �� ����
	virtual bool Start();

	/// �ʱ�ȭ
	virtual void Init(CNetworkMode::SConfigInfo const& serverInfo_);

	/// ��Ʈ��ũ ��� ���̱�
	void AttachNetworkMode(CNetworkMode* mode_);

	/// ��Ʈ��ũ ��� ��������
	CNetworkMode* GetNetworkMode() const;

	/// serverTask ó��
	virtual bool Process(CTaskBase* task_);



public:
	CServerBase();
	virtual ~CServerBase();
};