#pragma once

#include "RecvTask.h"
#include "ThreadRegister.hpp"
#include "MemoryPool.hpp"
#include "MessageContextBase.h"
#include <map>

class CServerBase;
class CSession;
class CTaskBase;
class CReceiver :
	public CTaskThreadRegister<CReceiver>,
	public CMemoryPool<SRecvTask>
{
public:
	typedef std::map<packetdef::PACKET_NUMBER, CMessageContextBase*> CONTEXT_HANDLER_MAP;


private:
	CONTEXT_HANDLER_MAP m_ContextMessageMap;

public:
	CReceiver();
	~CReceiver();

public:
	bool Start();
	bool RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_);
	bool PushTask(__int32 index_, SRecvTask::SInfo& param_);
	
	bool DistributePacket(CSession* session_);
	bool ProcessMessage(SRecvTask* task_);
	inline bool IsRegistedMessaseContext() { return !m_ContextMessageMap.empty(); }

	/// 쓰레드 처리
	virtual bool Process(CTaskBase*) override;

	/// 종료
	virtual void Close() override;
};
