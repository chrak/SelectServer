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
	/// �ʱ�ȭ
	void InitInfo(SInfo const& info_);

	/// �޽��� ���ؽ�Ʈ ���
	bool RegistContext(packetdef::PacketRegion const region_, CMessageContextBase* context_);

	/// Thread ��� �� ����
	virtual bool Start();

	/// serverTask ó��
	virtual bool Process(CTaskBase* task_);

	/// ���� ó��
	virtual void Close() override;
};
