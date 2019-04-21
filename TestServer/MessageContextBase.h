#pragma once
#include "Session.h"
#include "Packet.h"

class CMessageContextBase
{
public:
	CMessageContextBase();
	virtual ~CMessageContextBase();

public:
	virtual bool MessageProc(CSession::INDEX, packets::PacketID, void*) = 0;

#define DESERIALIZE(packet, buff) \
	packets::Get##packet(##buff) 
};
