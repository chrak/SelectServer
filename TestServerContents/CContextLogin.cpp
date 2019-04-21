#include "pch.h"
#include "CContextLogin.h"
#include "SessionManager.h"
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"
#include "../Packet/LoginAck_generated.h"

using namespace packetdef;


CContextLogin::CContextLogin()
{
}

CContextLogin::~CContextLogin()
{
}

bool CContextLogin::MessageProc(CSession::INDEX index_, packetdef::PacketID packetId_, void* buff_)
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
			CSessionManager::GetInstance()->SendSession(index_, packetId_, builder);
				
		}
		break;

	default:
		return false;
	}
	
	return true;
}