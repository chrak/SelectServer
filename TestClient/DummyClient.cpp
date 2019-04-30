#include "pch.h"
#include "DummyClient.h"
#include "DummyCommandTask.h"
#include "SessionManager.h"
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"
#include "../Packet/MessageReq_generated.h"


void CDummyClient::DerivedInit()
{
	CSessionManager<CConnector>::GetInstance()->Init();
}
	
void CDummyClient::DerivedRelease()
{
	Close();
	CSessionManager<CConnector>::GetInstance()->Release();
}

void CDummyClient::InitInfo(SInfo const& info_)
{
	m_DummyClientInfo.ConnectionAddr = info_.ConnectionAddr;
	m_DummyClientInfo.PortNumber = info_.PortNumber;
	m_DummyClientInfo.dummyCount = info_.dummyCount;
}


bool CDummyClient::RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_)
{
	auto result = CSessionManager<CConnector>::GetInstance()->ExecuteSessionAll([&](CSessionManager<CConnector>::SESSION_MAP const& sessionMap) -> bool {
		for (auto pair : sessionMap)
		{
			auto connector = static_cast<CConnector*>(pair.second);
			if (!connector->GetRecieverRef().RegistContext(region_, context_))
			{
				return false;
			}
		}

		return true;
	});

	return result;
}


bool CDummyClient::Start()
{
	auto dummyCount = m_DummyClientInfo.dummyCount;

	CConnector::SInfo info;
	info.ConnectionAddr = m_DummyClientInfo.ConnectionAddr;
	info.PortNumber = m_DummyClientInfo.PortNumber;
	info.ReceiverThreadCount = 1;

	for (auto i = 0; i < dummyCount; ++i)
	{
		auto connector = ALLOCATE_NEW(CConnector);
		
		/// 별로 좋은 방법은 아니지만...
		if (!CSessionManager<CConnector>::GetInstance()->RegistSession(connector))
		{
			SAFE_DELETE(connector);
			return false;
		}

		connector->Start(info);
	}

	m_IsRunning = true;
	__super::RegisterThreadFunc(1, true);

	return true;
}


bool CDummyClient::Process(CTaskBase* task_)
{
	if (task_ == NULL)
	{
		return false;
	}

	/// 여기서 send 명령 수행
	auto task = static_cast<CDummyCommandTask*>(task_);

	switch (task->CommandType)
	{
	case CDummyCommandTask::CHAT_TYPE:
		{
			flatbuffers::FlatBufferBuilder builder;
			auto userId = 17;
			auto message = builder.CreateString("Hello!!");
			auto req = packets::CreateMessageReq(builder, userId, message);
			builder.Finish(req);
			CSessionManager<CConnector>::GetInstance()->SendSessionAll(packetdef::MessageReq, builder);
		}
		break;

	case CDummyCommandTask::LOGIN_TYPE:
		{
			flatbuffers::FlatBufferBuilder builder;
			auto id = builder.CreateString("Test!!");
			auto pass = builder.CreateString("Hello!!");
			auto req = packets::CreateLoginReq(builder, id, pass);
			builder.Finish(req);
			CSessionManager<CConnector>::GetInstance()->SendSessionAll(packetdef::LoginReq, builder);
		}
		break;

	case CDummyCommandTask::EXIT_TYPE:
		{
			m_IsRunning = false;
		}
		break;

	}

	return true;
}

	
void CDummyClient::Close()
{
	__super::Close();
	CSessionManager<CConnector>::GetInstance()->Release();
}

