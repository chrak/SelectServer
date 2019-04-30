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
		// �α�
		return false;
	}

	SInfo* info = static_cast<CDummyCommandTask::SInfo*>(param_);
	CommandType = info->CommandType;
	CommondMessage = info->CommandMessage;  ///< �̰� ���� ������ ���� ���굥...

	return true;
}

bool CDummyCommandTask::Retrieve()
{
	return true;
}