#pragma once
#include "pch.h"
#include "stdafx.h"
#include "SessionManager.h"
#include "MemoryPool.hpp"



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
	std::lock_guard<std::mutex> lock(m_Mutex);   //< scoped_lock�� �� �ֽŰ����� �� IDE���� ���� ����...
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


bool CSessionManager::ExecuteSession(FUNC_SESSION_PROC func_)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	return func_(m_SessionMap);
}


