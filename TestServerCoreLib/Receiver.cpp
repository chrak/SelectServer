#include "Receiver.h"
#include "TaskBase.h"
#include "ServerBase.h"
#include "Session.h"
#include "PacketDef.h"
#include "Structure.h"


CReceiver::CReceiver()
	: CMemoryPool<SRecvTask>(100, 10)
{

}

CReceiver::~CReceiver()
{
}


bool CReceiver::Start()
{
	__super::RegisterThreadFunc(5, true);

	return true;
}


bool CReceiver::RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_)
{
	auto result = m_ContextMessageMap.insert(std::make_pair(region_, context_));
	return result.second;
}


bool CReceiver::DistributePacket(CSession* session_)
{
	packets::SHeader* header = NULL;;

	auto remainLen = session_->GetRecvRef().RemainLen;
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

	session_->GetRecvRef().RemainLen -= readPos;
	session_->GetRecvRef().PrevRecvPos = readPos;

	return true;
}


bool CReceiver::PushTask(__int32 index_, SRecvTask::SInfo& param_)
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


bool CReceiver::Process(CTaskBase* task_)
{
	auto task = static_cast<SRecvTask*>(task_);
	auto result = ProcessMessage(task);

	return result;
}


bool CReceiver::ProcessMessage(SRecvTask* task_)
{
	packetdef::PACKET_NUMBER region = 0xff00 & task_->PacketId;

	auto itor = m_ContextMessageMap.find(region);
	if (itor == m_ContextMessageMap.end())
	{
		return false;
	}

	auto index = task_->Index;
	auto packetId = task_->PacketId;
	auto buffer = task_->pBuffer;
	(*itor).second->MessageProc(index, packetId, buffer);

	return true;
}


void CReceiver::Close()
{
	__super::Close();
	
	for (auto pair : m_ContextMessageMap) 
	{
		auto context = pair.second;
		SAFE_DELETE(context);
	}
}

