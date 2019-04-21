#pragma once
#include "SessionManager.h"
#include "Singleton.hpp"
#include "MemoryPool.hpp"

template<typename T>
void CSessionManager<T>::DerivedInit()
{
	m_pSessionPool = ALLOCATE_NEW(CMemoryPool<CSession>, FD_SETSIZE, 10);
	m_SessionCount = 0;
	m_CurrentIndex = 0;
}


template<typename T>
void CSessionManager<T>::DerivedRelease()
{
	m_pSessionPool->ReleasePool();
}


template<typename T>
void CSessionManager<T>::ReleaseSession(CSession* session_)
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);

	if (session_ == NULL)
	{
		return;
	}

	::closesocket(session_->GetSocket());
	m_SessionMap.erase(session_->GetIndex());
	m_pSessionPool->RetrievePoolObject(session_);

	--m_SessionCount;
}

template<typename T>
CSession* CSessionManager<T>::NewSession(SOCKET socket_)
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);   //< scoped_lock이 더 최신같은데 내 IDE에서 지원 안함...
	++m_CurrentIndex;

	CSession::SInfo info;
	info.Socket = socket_;
	info.Index = m_CurrentIndex;

	T* obj = NULL;
	if (m_pSessionPool->AcquirePoolObject(obj, &info))
	{
		m_SessionMap.insert(std::make_pair(m_CurrentIndex, obj));
	}

	auto session = static_cast<CSession*>(obj);

	return session;
}


template<typename T>
bool CSessionManager<T>::ExecuteSessionAll(FUNC_SESSION_ALL_PROC func_)
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);

	return func_(m_SessionMap);
}


template<typename T>
bool CSessionManager<T>::ExecuteSession(CSession::INDEX index_, FUNC_SESSION_PROC func_)
{
	std::lock_guard<std::recursive_mutex> lock(m_Mutex);

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


template<typename T>
bool CSessionManager<T>::SendSessionAll(packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
{
	return ExecuteSessionAll([&](SESSION_MAP const& sessionMap) -> bool {
		for (auto pair : sessionMap)
		{
			auto session = pair.second;
			session->SendPacket(packetId_, builder_);
		}

		return true;
	});
}


template<typename T>
bool CSessionManager<T>::SendSession(CSession::INDEX index_, packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
{
	return ExecuteSession(index_, [&](CSession* session) -> bool {
		return session->SendPacket(packetId_, builder_);
	});
}

