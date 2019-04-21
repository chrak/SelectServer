#pragma once
#include "stdafx.h"

class CPoolObject
{
public:
	CPoolObject() {};
	virtual ~CPoolObject() {};

public:
	virtual bool Acquire(void*) = 0;
	virtual bool Retrieve() = 0;
};