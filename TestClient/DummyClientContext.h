#pragma once
#include "MessageContextBase.h"

class CDummyClientContext :
	public CMessageContextBase
{
public:
	CDummyClientContext();
	~CDummyClientContext();

public:
	virtual bool MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_);
};