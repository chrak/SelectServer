#pragma once
#include "stdafx.h"
#include "Session.h"
#include "Receiver.h"
#include <string>


class CConnector : 
	public CSession
{
public:
	struct SInfo
	{
		std::string ConnectionAddr;
		__int16 PortNumber;
		__int32 ReceiverThreadCount;
	};

public:
	CConnector();
	~CConnector();

private:
	SInfo     m_connectionInfo;
	SOCKET    m_Socket;
	CReceiver m_Receiver;

public:

	/// ��ϵǾ� �ִ� Ŀ�ؼ� ������ ������ ���� �� recv, send ���� �����Ѵ�
	bool Start(SInfo const& init_);

	/// Ŀ�ؼ��� �ݰ� ���� ������ �����Ѵ�
	void Close();

	/// �ش� Ŀ�ؼ��� ���ù� ȹ��
	inline CReceiver& GetRecieverRef() { return m_Receiver; }

	/// �ش� Ŀ�ؼ��� ������ ������ �´�
	inline SOCKET const GetSocket() { return m_Socket; }


	virtual bool Acquire(void*) override;
	virtual bool Retrieve() override;

};
