#pragma once
#include "MessageContextBase.h"

class CLoginContext
	: public CMessageContextBase
{
public:
	CLoginContext();
	~CLoginContext();

public:
	virtual bool MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_);
};
