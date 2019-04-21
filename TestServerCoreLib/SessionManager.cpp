#pragma once
#include "pch.h"
#include "stdafx.h"
#include "SessionManager.h"


void CSessionManager::DerivedInit()
{
	m_pSessionPool = ALLOCATE_NEW(CMemoryPool<CSession>, FD_SETSIZE, 0);
	m_SessionCount = 0;
	m_CurrentIndex = 0;
}

void CSessionManager::DerivedRelease()
{
	m_pSessionPool->ReleasePool();
}

CSession* CSessionManager::NewSession(SOCKET socket_)
{
	std::lock_guard<std::mutex> lock(m_Mutex);   //< scoped_lock이 더 최신같은데 내 IDE에서 지원 안함...
	++m_CurrentIndex;

	CSession::SInfo info;
	info.Socket = socket_;
	info.Index = m_CurrentIndex;

	CSession* session = NULL;
	if (m_pSessionPool->AcquirePoolObject(session, &info))
	{
		m_SessionMap.insert(std::make_pair(m_CurrentIndex, session));
	}

	return session;
}

void CSessionManager::ReleaseSession(CSession* session_)
{
	std::lock_guard<std::mutex> lock(m_Mutex); 

	::closesocket(session_->GetSocket());
	m_SessionMap.erase(session_->GetIndex());
	m_pSessionPool->RetrievePoolObject(session_);

	--m_SessionCount;
}


bool CSessionManager::ExecuteSessionAll(FUNC_SESSION_ALL_PROC func_)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	return func_(m_SessionMap);
}

bool CSessionManager::ExecuteSession(CSession::INDEX index_, FUNC_SESSION_PROC func_)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto itor = m_SessionMap.find(index_);
	if (itor == m_SessionMap.end())
	{
		return false;
	}

	CSession* session = (*itor).second;
	if (session == NULL)
	{
		return false;
	}

	return func_(session);
}


bool CSessionManager::SendSession(CSession::INDEX index_, packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
{
	return ExecuteSession(index_, [&](CSession* session) -> bool {
		return session->SendPacket(packetId_, builder_);
	});
}

