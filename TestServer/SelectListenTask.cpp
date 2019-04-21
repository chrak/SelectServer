#include "pch.h"
#include "SelectListenTask.h"


CSelectListenTask::CSelectListenTask()
{

}


CSelectListenTask::~CSelectListenTask()
{

}

void CSelectListenTask::SetData(SOCKET const& socket_)
{
	m_Socket = socket_;	
}
