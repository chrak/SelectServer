#include "pch.h"
#include "ServerBase.h"
#include "ThreadRegister.hpp"
#include <winsock2.h>


bool CServerBase::Start()
{
	if (!m_IsInit)
	{
		// �α�
		return false;
	}

	if (m_pNetworkMode == NULL)
	{
		// �α�
		return false;
	}

	WORD versionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(versionRequested, &wsaData);
	__super::RegisterThreadFunc(1, false);

	return true;
}

void CServerBase::AttachNetworkMode(CNetworkMode* mode_)
{
	if (mode_ == NULL)
	{
		// �α�
		return;
	}

	m_pNetworkMode = mode_;
}

CNetworkMode* CServerBase::GetNetworkMode() const
{
	return m_pNetworkMode;
}

bool CServerBase::Process(CTaskBase* task_)
{
	m_pNetworkMode->TransferProcess(this);

	return true;
}
