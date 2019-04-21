#pragma once

#include "RecvTask.h"
#include "ThreadRegister.hpp"
#include "MemoryPool.hpp"
#include "MessageContextBase.h"
#include <map>

class CServerBase;
class CSession;
class CTaskBase;
class CReciever :
	public CTaskThreadRegister<CReciever>,
	public CMemoryPool<SRecvTask>
{
public:
	typedef std::map<packets::PACKET_NUMBER, CMessageContextBase*> CONTEXT_HANDLER_MAP;

private:
	CServerBase* m_pServer;  ///< 서버
	CONTEXT_HANDLER_MAP m_ContextMessageMap;

public:
	CReciever(CServerBase* server_);
	~CReciever();

public:
	bool Start();

	bool RegistContext(packets::PacketRegion const region_, CMessageContextBase* context_);
	bool PushTask(__int32 index_, SRecvTask::SInfo& param_);
	bool DistributePacket(CSession* session_);

	/// 쓰레드 처리
	virtual bool Process(CTaskBase*) override;
};
