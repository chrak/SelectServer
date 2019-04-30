#pragma once
#include "Singleton.hpp"
#include "Connector.h"
#include "PacketDef.h"
#include "ThreadRegister.hpp"
#include <string>

class CMessageContextBase;
class CDummyClient :
	public CSingleton<CDummyClient>,
	public CTaskThreadRegister<CDummyClient>
{
public:
	struct SInfo
	{
		std::string ConnectionAddr;
		__int16 PortNumber;
		__int32 dummyCount;
	};

private:
	SInfo m_DummyClientInfo;

public:
	virtual void DerivedInit() override;
	virtual void DerivedRelease() override;

public:
	/// 초기화
	void InitInfo(SInfo const& info_);

	/// 메시지 컨텍스트 등록
	bool RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_);

	/// Thread 등록 및 시작
	virtual bool Start();

	/// serverTask 처리
	virtual bool Process(CTaskBase* task_);

	/// 종료 처리
	virtual void Close() override;
};
