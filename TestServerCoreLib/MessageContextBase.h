#pragma once
#include "Session.h"
#include "PacketDef.h"

class CMessageContextBase
{
public:
	CMessageContextBase() {}
	virtual ~CMessageContextBase() {}

public:
	virtual bool MessageProc(CSession::INDEX, packetdef::PacketID, void*) = 0;

#define DESERIALIZE(packet, buff) \
	packets::Get##packet(##buff) 
};
