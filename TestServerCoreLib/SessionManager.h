#pragma once
#include <map>
#include <mutex>
#include <functional>
#include <winsock2.h>
#include "PacketDef.h"
#include "Session.h"
#include "Singleton.hpp"
#include "MemoryPool.hpp"
#include "flatbuffers/flatbuffers.h"

template<typename T>
class CSessionManager :
	public CSingleton<CSessionManager<T>>
{
public:
	typedef std::map<CSession::INDEX, CSession*> SESSION_MAP;
	typedef CMemoryPool<T>* SESSION_POOL;

	typedef std::function<bool(SESSION_MAP const&)> FUNC_SESSION_ALL_PROC;
	typedef std::function<bool(CSession*)> FUNC_SESSION_PROC;

private:
	std::recursive_mutex m_Mutex;
	SESSION_MAP m_SessionMap;
	SESSION_POOL m_pSessionPool;

	__int32 m_SessionCount;
	__int32 m_CurrentIndex;

private:
	virtual void DerivedInit() override
	{
		m_pSessionPool = ALLOCATE_NEW(CMemoryPool<T>, FD_SETSIZE, 10);
		m_SessionCount = 0;
		m_CurrentIndex = 0;
	}
	virtual void DerivedRelease() override
	{
		m_pSessionPool->ReleasePool();
	}

public:
	CSession* NewSession(SOCKET socket_)
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

	bool RegistSession(CSession* session_)
	{
		std::lock_guard<std::recursive_mutex> lock(m_Mutex); 
		if (session_ == NULL)
		{
			return false;
		}
		
		++m_CurrentIndex;

		CSession::SInfo info;
		info.Socket = session_->GetSocket();
		info.Index = m_CurrentIndex;
		if (!session_->Acquire(&info))
		{
			// 로그
			return false;
		}

		auto result = m_SessionMap.insert(std::make_pair(m_CurrentIndex, session_));
		return result.second;
	}

	void ReleaseSession(CSession* session_)
	{
		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		if (session_ == NULL)
		{
			return;
		}

		::closesocket(session_->GetSocket());
		m_SessionMap.erase(session_->GetIndex());
		m_pSessionPool->RetrievePoolObject(static_cast<T*>(session_));

		--m_SessionCount;
	}

	bool ExecuteSessionAll(FUNC_SESSION_ALL_PROC func_)
	{
		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		return func_(m_SessionMap);
	}

	bool ExecuteSession(CSession::INDEX index_, FUNC_SESSION_PROC func_)
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

	bool SendSessionAll(packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
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

	bool SendSession(CSession::INDEX index_, packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
	{
		return ExecuteSession(index_, [&](CSession* session) -> bool {
			return session->SendPacket(packetId_, builder_);
		});
	}
};