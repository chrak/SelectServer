#include "pch.h"
#include "RecvTask.h"


SRecvTask::SRecvTask()
{
}


SRecvTask::~SRecvTask()
{
}


bool SRecvTask::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// ·Î±×
		return false;
	}

	SInfo* info = static_cast<SRecvTask::SInfo*>(param_);
	Index = info->Index;
	Length = info->Length;
	PacketId = info->PacketId;
	::memcpy_s(pBuffer, Length, info->pBuffer, Length); 

	return true;
}

bool SRecvTask::Retrieve()
{

	return true;
}