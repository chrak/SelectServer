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
// 서버 기본 class
///////////////////////////////////////////////////////////////////////////
class CServerBase :
	public CTaskThreadRegister<CServerBase>
{
protected:
	ServerType m_ServerType;
	CNetworkMode::SConfigInfo m_ServerInfo;           ///< 서버 정보 구조체

	SOCKADDR_IN m_ServerAddr;
	CListener m_Listener;
	CReceiver m_Receiver;
	CNetworkMode* m_pNetworkMode;       ///< 네트워크 모드

	bool m_IsInit;                ///< 초기화 완료 유무
	bool m_IsRunning;             ///< 쓰레드 실행중 유무

public:
	/// 리스너 레퍼런스 획득
	inline CListener& GetListenerRef() { return m_Listener; }

	/// 리시버 레퍼런스 획득
	inline CReceiver& GetRecieverRef() { return m_Receiver; }

	/// 서버타입 획득
	inline ServerType GetServerType() { return m_ServerType; }

	/// 메시지 컨텍스트 등록
	bool RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_);

	/// 초기화
	virtual void Init(CNetworkMode::SConfigInfo const& serverInfo_);

	/// Thread 등록 및 시작
	virtual bool Start();

	/// 네트워크 모드 붙이기
	void AttachNetworkMode(CNetworkMode* mode_);

	/// 네트워크 모드 가져오기
	CNetworkMode* GetNetworkMode() const;

	/// serverTask 처리
	virtual bool Process(CTaskBase* task_);

	/// 종료 처리
	virtual void Close() override;


public:
	CServerBase();
	virtual ~CServerBase();
};