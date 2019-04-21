#include "pch.h"
#include "RecvTask.h"

SRecvTask::SRecvTask() :
	Index(0),
	Length(0),
	PacketId(packetdef::PacketID::Invalid)
{
}


SRecvTask::~SRecvTask()
{
}


bool SRecvTask::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// 로그
		return false;
	}

	SInfo* info = static_cast<SRecvTask::SInfo*>(param_);
	Index = info->Index;
	Length = info->Length;
	PacketId = info->PacketId;
	if (Length > PACKET_SIZE)
	{
		// 로그
		return false;
	}

	::memcpy_s(pBuffer, Length, info->pBuffer, Length); 

	return true;
}

bool SRecvTask::Retrieve()
{
	return true;
}