#pragma once
#include "PacketDef.h"

#pragma pack(push, 1)

namespace packets
{
	struct SHeader
	{
		packetdef::PacketID id;
		__int16 dataSize;
		__int64 version;
		__int16 reserv;

		SHeader()
		{
		}

		SHeader(packetdef::PacketID id_, __int16 size_) :
			id(id_),
			dataSize(size_),
			version(0),
			reserv(0)
		{
		}
	};

	const int PACKET_HEADER_SIZE = sizeof(SHeader);
}

#pragma pack(pop)

