#include "pch.h"
#include "LoginContext.h"
#include "SessionManager.h"
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"
#include "../Packet/LoginAck_generated.h"

#include "../Packet/MessageReq_generated.h"
#include "../Packet/MessageNfy_generated.h"


using namespace packetdef;


CLoginContext::CLoginContext()
{
}

CLoginContext::~CLoginContext()
{
}

bool CLoginContext::MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_)
{
	switch (packetId_)
	{
	case packetdef::LoginReq:
		{
			auto req = DESERIALIZE(LoginReq, buff_);
			auto id = req->id()->c_str();
			auto pass = req->pass()->c_str();

			flatbuffers::FlatBufferBuilder builder;
			auto serial = static_cast<int32_t>(index_);
			auto userID = builder.CreateString(id);
			auto result = packets::ResultType_RESULT_SUCCESS;

			auto ack = packets::CreateLoginAck(builder, serial, userID, result);
			builder.Finish(ack);
			CSessionManager<CSession>::GetInstance()->SendSession(index_, packetId_, builder);
		}
		break;

	case packetdef::MessageReq:
		{
			auto req = DESERIALIZE(MessageReq, buff_);
			auto userId = req->userId();
			auto message = req->message()->c_str();

			flatbuffers::FlatBufferBuilder builder;
			auto serial = static_cast<int32_t>(index_);
			auto userName = builder.CreateString("¾Æ¹«°³");
			auto messageNty = builder.CreateString(message);

			auto ack = packets::CreateMessageNfy(builder, serial, userName, messageNty);
			builder.Finish(ack);
			CSessionManager<CSession>::GetInstance()->SendSessionAll(packetId_, builder);
		}
		break;


	default:
		return false;
	}
	
	return true;
}