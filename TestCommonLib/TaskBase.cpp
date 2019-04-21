#include "stdafx.h"
#include "TaskBase.h"

CTaskBase::CTaskBase()
{

}

CTaskBase::~CTaskBase()
{

}

bool CTaskBase::Run()
{
	return false;
}

bool CTaskBase::Acquire(void*)
{ 
	return false; 
}

bool CTaskBase::Retrieve()
{ 
	return false; 
}
