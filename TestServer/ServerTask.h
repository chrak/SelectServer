#pragma once

#include "TaskBase.h"

class CServerTask : 
	public CTaskBase
{
public:
	CServerTask();
	~CServerTask();

public:
	/// ���� �Լ�
	virtual bool Run() override;
};