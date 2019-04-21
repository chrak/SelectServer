#pragma once
#include "TaskBase.h"
#include <WinSock2.h>

class CSelectListenTask
	: public CTaskBase
{
private:
	SOCKET m_Socket;

public:
	void SetData(SOCKET const& socket_);
	inline SOCKET GetSocket() { return m_Socket;  }

public:
	CSelectListenTask();
	~CSelectListenTask();
};
