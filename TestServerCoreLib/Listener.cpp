#include "pch.h"
#include "Listener.h"
#include "ServerBase.h"


CListener::CListener(CServerBase* server_)
	: m_pServer(server_)
{

}

CListener::~CListener()
{

}

bool CListener::Start(CNetworkMode::SConfigInfo const& config_)
{
	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(config_.PortNumber);
	m_ListenPort = config_.PortNumber;
	m_pServer->GetNetworkMode()->SetSockOption(m_ListenSocket);

	if (::bind(m_ListenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// 로그
		return NULL;
	}

	if (::listen(m_ListenSocket, config_.BackLogSize) == SOCKET_ERROR)
	{
		// 로그
		return NULL;
	}

	m_pServer->GetNetworkMode()->Init(m_ListenSocket, config_);
	//__super::RegisterThreadFunc(1, false);

	return m_ListenSocket;
}



bool CListener::Process(CTaskBase* task_)
{
	/// select 방식에서는 무쓸모....
//	auto networkMode = m_pServer->GetNetworkMode();
//	networkMode->LoginProcess(task_);

	return true;
}


