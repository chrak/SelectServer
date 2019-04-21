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
// 서버 기본 class
///////////////////////////////////////////////////////////////////////////
class CServerBase :
	public CTaskThreadRegister<CServerBase>
{
public:
	typedef std::map<packets::PacketRegion, CMessageContextBase*> CONTEXT_HANDLER_MAP;

protected:
	CNetworkMode::SConfigInfo m_ServerInfo;           ///< 서버 정보 구조체
	CONTEXT_HANDLER_MAP m_ContextMap;

	SOCKADDR_IN m_ServerAddr;
	CListener m_Listener;
	CReciever m_Receiver;

	CNetworkMode* m_pNetworkMode;       ///< 네트워크 모드

	int server_sockfd, client_sockfd;
	int state, client_len;
	int client[FD_SETSIZE];


	bool m_IsInit;                ///< 초기화 완료 유무
	bool m_IsRunning;             ///< 쓰레드 실행중 유무

public:
	/// 리스너 레퍼런스 획득
	CListener& const GetListenerRef() { return m_Listener; }

	/// 리시버 레퍼런스 획득
	CReciever& const GetRecieverRef() { return m_Receiver; }

	/// 메시지 컨텍스트 등록
	bool RegistContext(packets::PacketRegion const region_, CMessageContextBase* context_);

	/// serverTask Thread 등록 및 시작
	virtual bool Start();

	/// 초기화
	virtual void Init(CNetworkMode::SConfigInfo const& serverInfo_);

	/// 네트워크 모드 붙이기
	void AttachNetworkMode(CNetworkMode* mode_);

	/// 네트워크 모드 가져오기
	CNetworkMode* GetNetworkMode() const;

	/// serverTask 처리
	virtual bool Process(CTaskBase* task_);



public:
	CServerBase();
	virtual ~CServerBase();
};