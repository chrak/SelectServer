#pragma once

#include "TaskBase.h"
#include "PacketDef.h"
#include <string>

struct CDummyCommandTask
	: public CTaskBase
{
	enum E_TYPE
	{
		NONE,
		LOGIN_TYPE,
		CHAT_TYPE,
	};

	struct SInfo
	{
		E_TYPE CommandType;
		std::string CommandMessage;
	};

	E_TYPE CommandType;
	std::string CommondMessage;
	
	
	CDummyCommandTask();
	~CDummyCommandTask();

	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;
};