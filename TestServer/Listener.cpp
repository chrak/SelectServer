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

	//LINGER ls;
	//ls.l_onoff = 1;
	//ls.l_linger = 0;
	//::setsockopt(m_listenSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char *>(&ls), sizeof(ls));
	bool bValid = true;
	::setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&bValid), sizeof(bValid));

	if (::bind(m_ListenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// 로그
		return NULL;
	}

	if (::listen(m_ListenSocket, FD_SETSIZE) == SOCKET_ERROR)
	{
		// 로그
		return NULL;
	}

	//m_pNetworkMode->Init(m_ListenSocket, config_);
	__super::RegisterThreadFunc(1, false);

	return m_ListenSocket;
}



bool CListener::Process(CTaskBase* task_)
{
//	auto networkMode = m_pServer->GetNetworkMode();
//	networkMode->LoginProcess(task_);

	return true;
}


