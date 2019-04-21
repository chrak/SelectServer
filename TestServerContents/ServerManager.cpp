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

	/// 이곳에서 싱글턴을 사용하는 클래스들을 모두 초기화한다.
	CSessionManager::Init();
}

void CServerManager::DerivedRelease()
{
	Close();
	::WSACleanup();

	/// 이곳에서 싱글턴을 사용하는 클래스들을 모두 해제한다.
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
		// 로그
		return false;
	}

	server->Init(info_);

	auto result = m_ServerMap.insert(std::make_pair(info_.ServerType, server));
	if (!result.second)
	{
		// 로그
		return false;
	}

	return true;
}

bool CServerManager::RemoveServer(ServerType const serverType_)
{
	if (m_ServerMap.find(serverType_) == m_ServerMap.end())
	{
		// 로그
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
			// 로그
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
