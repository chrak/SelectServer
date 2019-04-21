#pragma once

#include "TaskBase.h"

class CServerTask : 
	public CTaskBase
{
public:
	CServerTask();
	~CServerTask();

public:
	/// 수행 함수
	virtual bool Run() override;
};