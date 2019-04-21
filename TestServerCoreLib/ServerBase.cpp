#include "pch.h"
#include "ServerBase.h"
#include <winsock2.h>


CServerBase::CServerBase()
	: m_Listener(this),
	  m_Receiver()
{
}

CServerBase::~CServerBase()
{

}


/// 메시지 컨텍스트 등록
bool CServerBase::RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_)
{
	auto result = m_Receiver.RegistContext(region_, context_);
	return result;
}

/// 초기화
void CServerBase::Init(CNetworkMode::SConfigInfo const& serverInfo_)
{
	::memcpy_s(&m_ServerInfo, sizeof(CNetworkMode::SConfigInfo), &serverInfo_, sizeof(CNetworkMode::SConfigInfo));
	m_IsInit = true;
}


bool CServerBase::Start()
{
	if (!m_IsInit)
	{
		// 로그
		return false;
	}

	if (m_pNetworkMode == NULL)
	{
		// 로그
		return false;
	}

	m_Listener.Start(m_ServerInfo);
	m_Receiver.Start();

	__super::RegisterThreadFunc(1, false);

	return true;
}

void CServerBase::AttachNetworkMode(CNetworkMode* mode_)
{
	if (mode_ == NULL)
	{
		// 로그
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

void CServerBase::Close()
{
	__super::Close();
	SAFE_DELETE(m_pNetworkMode);
}
