#pragma once
#include "MessageContextBase.h"

class CContextLogin
	: public CMessageContextBase
{
public:
	CContextLogin();
	~CContextLogin();

public:
	virtual bool MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_);
};
