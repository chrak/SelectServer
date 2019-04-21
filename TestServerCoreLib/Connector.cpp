#include "Connector.h"
#include "Session.h"


CConnector::CConnector()
	: m_Receiver()
{
}

CConnector::~CConnector()
{
}


bool CConnector::Start(SInfo const& info_)
{
	if (!m_Receiver.IsRegistedMessaseContext())
	{
		// 로그
		return false;
	}

	// 서버 주소
	sockaddr_in ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(ServerAddr));

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(info_.PortNumber);
	ServerAddr.sin_addr.s_addr = inet_addr(info_.ConnectionAddr.c_str());

	//	u_long on = TRUE;
	//	int retval = ::ioctlsocket(m_pSession->GetSocket(), FIONBIO, &on);

	if (connect(m_Socket, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) != 0)
	{
		// 로그
		
		return false;
	}

	if (!m_Receiver.Start())
	{
		// 로그
		Close();
		return false;
	}

	return true;
}


/// 커넥션을 닫고 관련 동작을 종료한다
void CConnector::Close()
{
	m_Receiver.Close();
}


bool CConnector::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// 로그
		return false;
	}

	CSession::SInfo* info = static_cast<CSession::SInfo*>(param_);
	m_Socket = info->Socket;
	m_Index = info->Index;
	m_IsConnected = true;  ///< 중간에 실패해도 Retrieve에 들어가게 한다면 상관없다

	return true;
}


bool CConnector::Retrieve()
{
	m_IsConnected = false;
	Close();

	return true;
}