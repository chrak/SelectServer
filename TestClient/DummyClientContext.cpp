#include "pch.h"
#include "DummyClientContext.h"
#include "SessionManager.h"
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginAck_generated.h"
#include "../Packet/MessageNfy_generated.h"

CDummyClientContext::CDummyClientContext()
{

}


CDummyClientContext::~CDummyClientContext()
{

}


bool CDummyClientContext::MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_)
{
	switch (packetId_)
	{
	case packetdef::LoginAck:
	{
		auto req = DESERIALIZE(LoginAck, buff_);
		auto serial = req->serial();
		auto userName = req->id()->c_str();
		auto result = req->result();
	}
	break;

	case packetdef::MessageNfy:
	{
		auto req = DESERIALIZE(MessageNfy, buff_);
		auto userId = req->userId();
		auto message = req->message()->c_str();

		flatbuffers::FlatBufferBuilder builder;
		auto serial = static_cast<int32_t>(index_);
		auto userName = builder.CreateString("¾Æ¹«°³");
		auto messageNty = builder.CreateString(message);

	}
	break;


	default:
		return false;
	}

	return true;
}
