#pragma once

#include "stdafx.h"

namespace packetdef
{	

	typedef unsigned short PACKET_NUMBER;

	typedef enum E_PACKET_REGION : unsigned short
	{
		REGION_LOGIN = 0x1000,
		REGION_ROOM  = 0x1100,
	} PacketRegion;

	typedef enum E_PACKET_ID : unsigned short
	{
		Invalid = 0,
		LoginReq = PacketRegion::REGION_LOGIN,
		LoginAck = PacketRegion::REGION_LOGIN + 1,
		MessageReq = PacketRegion::REGION_ROOM,
		MessageNfy = PacketRegion::REGION_ROOM + 1,
	} PacketID;
}



