#include "Receiver.h"
#include "TaskBase.h"
#include "ServerBase.h"
#include "Session.h"
#include "PacketDef.h"
#include "Structure.h"


CReciever::CReciever(CServerBase* server_)
	: m_pServer(server_),
	CMemoryPool<SRecvTask>(100, 10)
{

}

CReciever::~CReciever()
{

}


bool CReciever::Start()
{
	__super::RegisterThreadFunc(5, true);

	return true;
}

bool CReciever::RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_)
{
	auto result = m_ContextMessageMap.insert(std::make_pair(region_, context_));
	return result.second;
}


bool CReciever::PushTask(__int32 index_, SRecvTask::SInfo& param_)
{
	SRecvTask* task;
	if (AcquirePoolObject(task, &param_))
	{
		__int32 distIndex = index_ % GetThreadCount();
		__super::PushTask(distIndex, task);
		return true;
	}

	return false;
}


bool CReciever::DistributePacket(CSession* session_)
{
	packets::SHeader* header = NULL;;

	auto remainLen = session_->GetRecvRef().remainLen;
	auto readPos = 0;
	
	while (remainLen >= packets::PACKET_HEADER_SIZE)
	{
		/// 여기서 굳이 복사할 필요가 없다...
		header = reinterpret_cast<packets::SHeader*>(session_->GetRecvRef().pBuf + readPos);
		auto packetBodyLen = header->dataSize;

		if (packetBodyLen > remainLen)
		{
			/// 바디를 읽을 만큼의 데이터가 남아있지 않은 상황
			break;
		}

		if (packetBodyLen > PACKET_SIZE - packets::PACKET_HEADER_SIZE)
		{
			// 로그
			return false;
		}

		SRecvTask::SInfo info;
		info.Index = session_->GetIndex();
		info.PacketId = header->id;
		info.Length = packetBodyLen;
		info.pBuffer = session_->GetRecvRef().pBuf + readPos + packets::PACKET_HEADER_SIZE;   ///< 다른 쓰레드로 넘어가므로 task 안에서 복사 필요

		readPos += (packets::PACKET_HEADER_SIZE + packetBodyLen);
		remainLen -= readPos;

		PushTask(session_->GetIndex(), info);  
	}

	session_->GetRecvRef().remainLen -= readPos;
	session_->GetRecvRef().prevRecvPos = readPos;

	return true;
}


bool CReciever::Process(CTaskBase* task_)
{
	auto task = static_cast<SRecvTask*>(task_);

	packetdef::PACKET_NUMBER region = 0xff00 & task->PacketId;

	auto itor = m_ContextMessageMap.find(region);
	if (itor == m_ContextMessageMap.end())
	{
		return false;
	}

	auto index = task->Index;
	auto packetId = task->PacketId;
	auto buffer = task->pBuffer;
	(*itor).second->MessageProc(index, packetId, buffer);

	return true;
}


void CReciever::Close()
{
	__super::Close();
	
	for (auto pair : m_ContextMessageMap) 
	{
		auto context = pair.second;
		SAFE_DELETE(context);
	}
}

