#include "pch.h"
#include "DummyCommandTask.h"


CDummyCommandTask::CDummyCommandTask() :
	CommandType(E_TYPE::NONE)
{
}


CDummyCommandTask::~CDummyCommandTask()
{
}


bool CDummyCommandTask::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// 로그
		return false;
	}

	SInfo* info = static_cast<CDummyCommandTask::SInfo*>(param_);
	CommandType = info->CommandType;
	CommondMessage = info->CommandMessage;  ///< 이거 변경 없으면 얇은 복산데...

	return true;
}

bool CDummyCommandTask::Retrieve()
{
	return true;
}