#pragma once
#include "pch.h"
#include "ServerManager.h"
#include "ServerBase.h"
#include "ContentsServer.h"
#include "CContextLogin.h"
#include "NetworkSelectMode.h"
#include "PacketDef.h"
#include <WinSock2.h>


void CServerManager::DerivedInit()
{
	WORD versionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	::WSAStartup(versionRequested, &wsaData);

	/// �̰����� �̱����� ����ϴ� Ŭ�������� ��� �ʱ�ȭ�Ѵ�.
	CSessionManager::Init();
}

void CServerManager::DerivedRelease()
{
	Close();
	::WSACleanup();

	/// �̰����� �̱����� ����ϴ� Ŭ�������� ��� �����Ѵ�.
	CSessionManager::Release();
}


bool CServerManager::RegistServer(CNetworkMode::SConfigInfo const& info_)
{
	CServerBase* server = NULL;
	switch (info_.ServerType)
	{
	case ServerType::CONTENTS_SERVER_TYPE:
		server = ALLOCATE_NEW(CContentsServer);
		server->AttachNetworkMode(ALLOCATE_NEW(CNetworkSelectMode));
		server->RegistContext(packetdef::REGION_LOGIN, ALLOCATE_NEW(CContextLogin));
		break;
	}

	if (server == NULL)
	{
		// �α�
		return false;
	}

	server->Init(info_);

	auto result = m_ServerMap.insert(std::make_pair(info_.ServerType, server));
	if (!result.second)
	{
		// �α�
		return false;
	}

	return true;
}

bool CServerManager::RemoveServer(ServerType const serverType_)
{
	if (m_ServerMap.find(serverType_) == m_ServerMap.end())
	{
		// �α�
		return false;
	}

	m_ServerMap.erase(serverType_);
	return true;
}

bool CServerManager::Start()
{
	for (auto pair : m_ServerMap)
	{
		CServerBase* server = pair.second;
		if (!server->Start())
		{
			// �α�
			Close();
			return false;
		}
	}

	return true;
}

void CServerManager::Close()
{
	for (auto pair : m_ServerMap)
	{
		CServerBase* server = pair.second;
		server->Close();

		while (!server->IsClose())
		{
			::Sleep(1);
		}

		SAFE_DELETE(server);
	}

}
