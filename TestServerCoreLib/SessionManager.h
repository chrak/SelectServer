#pragma once
#include "stdafx.h"
#include <map>
#include <mutex>
#include <functional>
#include <winsock2.h>
#include "Session.h"
#include "Singleton.hpp"
#include "MemoryPool.hpp"


class CSessionManager :
	public CSingleton<CSessionManager>
{
public:
	typedef std::map<CSession::INDEX, CSession*> SESSION_MAP;
	typedef CMemoryPool<CSession>* SESSION_POOL;
	typedef std::function<bool(SESSION_MAP const&)> FUNC_SESSION_ALL_PROC;
	typedef std::function<bool(CSession*)> FUNC_SESSION_PROC;

private:
	std::mutex m_Mutex;
	SESSION_MAP m_SessionMap;
	SESSION_POOL m_pSessionPool;
	__int32 m_SessionCount;
	__int32 m_CurrentIndex;

private:
	virtual void DerivedInit() override;
	virtual void DerivedRelease() override;

public:
	CSession* NewSession(SOCKET socket_);
	void ReleaseSession(CSession* session_);
	bool ExecuteSessionAll(FUNC_SESSION_ALL_PROC func_);
	bool ExecuteSession(CSession::INDEX index_, FUNC_SESSION_PROC func_);
	bool SendSession(CSession::INDEX index_, packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_);
};