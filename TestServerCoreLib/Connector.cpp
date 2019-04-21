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
		// �α�
		return false;
	}

	// ���� �ּ�
	sockaddr_in ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(ServerAddr));

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(info_.PortNumber);
	ServerAddr.sin_addr.s_addr = inet_addr(info_.ConnectionAddr.c_str());

	//	u_long on = TRUE;
	//	int retval = ::ioctlsocket(m_pSession->GetSocket(), FIONBIO, &on);

	if (connect(m_Socket, (sockaddr*)&ServerAddr, sizeof(ServerAddr)) != 0)
	{
		// �α�
		
		return false;
	}

	if (!m_Receiver.Start())
	{
		// �α�
		Close();
		return false;
	}

	return true;
}


/// Ŀ�ؼ��� �ݰ� ���� ������ �����Ѵ�
void CConnector::Close()
{
	m_Receiver.Close();
}


bool CConnector::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// �α�
		return false;
	}

	CSession::SInfo* info = static_cast<CSession::SInfo*>(param_);
	m_Socket = info->Socket;
	m_Index = info->Index;
	m_IsConnected = true;  ///< �߰��� �����ص� Retrieve�� ���� �Ѵٸ� �������

	return true;
}


bool CConnector::Retrieve()
{
	m_IsConnected = false;
	Close();

	return true;
}