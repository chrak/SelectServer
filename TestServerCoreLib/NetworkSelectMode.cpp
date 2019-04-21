#include "pch.h"
#include "CommonDef.h"
#include "NetworkSelectMode.h"
#include "SessionManager.h"
#include "ServerBase.h"
#include "PacketDef.h"

//#include "flatbuffers/flatbuffers.h"
//#include "../Packet/LoginReq_generated.h"


CNetworkSelectMode::CNetworkSelectMode()
{
	m_TimeOut.tv_sec = SELECT_TIMEOUT_SEC;
	m_TimeOut.tv_usec = 0;
}
CNetworkSelectMode::~CNetworkSelectMode()
{

}

void CNetworkSelectMode::Init(SOCKET socket_, SConfigInfo const& config_)
{
	FD_ZERO(&m_MasterFd);
	FD_SET(socket_, &m_MasterFd);
	m_MaxSd = socket_;
}


bool CNetworkSelectMode::TransferProcess(CServerBase* server_)
{
	auto readSet = m_MasterFd;
	auto writeSet = m_MasterFd;
	//auto re = select(0, &readSet, &writeSet, 0, &m_TimeOut);
	auto re = select(0, &readSet, 0, 0, &m_TimeOut);
	if (re == -1)
	{
		// �α�
		return false;
	}
	else if (re == 0)
	{
		// Ÿ�Ӿƿ�
		return false;
	}

	auto listener = server_->GetListenerRef();
	auto readableCount = re;

	if (FD_ISSET(listener.GetSocket(), &readSet))
	{
		// �����带 �¿�ٰ� ������ �� fd_set�� �� �Ŵ� ��� ���� ������ ��� ������
		// auto task = ALLOCATE_NEW(CSelectListenTask);
		// task->SetData(listener.GetSocket());
		// listener.PushTask(task);
		
		LoginProcess(server_);

		--readableCount;
	}

	if (readableCount > 0)
	{
		CSessionManager::GetInstance()->ExecuteSessionAll([&](CSessionManager::SESSION_MAP sessionMap) -> bool{
			for (auto const & pair : sessionMap)
			{
				auto session = pair.second;

				/*
				select(0, 0, &writeSet, 0, 0);
				if (FD_ISSET(session->GetSocket(), &writeSet))
				{
					int aa = 0;
				}
				*/

				if (FD_ISSET(session->GetSocket(), &readSet))
				{
					if (RecvProcess(session))
					{
						server_->GetRecieverRef().DistributePacket(session);
					}
					
					--readableCount;
					if (readableCount <= 0)
					{
						break;
					}
				}			
			}

			return true;
		});
	}


	return true;
}


//bool CNetworkSelectMode::LoginProcess(CTaskBase* task_)
bool CNetworkSelectMode::LoginProcess(CServerBase* server_)
{
	//auto task = static_cast<CSelectListenTask*>(task_);

	auto listener = server_->GetListenerRef();
	
	SOCKADDR_IN client_addr;
	auto len = static_cast<__int32>(sizeof(client_addr));

	do
	{
		auto clientSocket = accept(listener.GetSocket(), (SOCKADDR*)&client_addr, &len);
		if (clientSocket == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// ���� ���۰� ����
				continue;
			}

			// �α�
			return false;
		}

		auto session = CSessionManager::GetInstance()->NewSession(clientSocket);
		if (session)
		{
			FD_SET(session->GetSocket(), &m_MasterFd);
		}
		else
		{
			// �α�
		}

	} while (false);

	return true;
}


bool CNetworkSelectMode::RecvProcess(CSession* session_)
{
	__int32 recvPos = 0;

	/// �����ִ� ���۰� �־��ٸ� �� ���۸� ���� �մܿ� �ΰ� �� �������� �ްԲ� ó�� 
	if (session_->GetRecvRef().remainLen > 0)
	{
		auto recvInfo = session_->GetRecvRef();

		::memcpy_s(&recvInfo.pBuf, recvInfo.remainLen, &recvInfo.pBuf + recvInfo.prevRecvPos, recvInfo.remainLen);
		recvPos += recvInfo.remainLen;
	}

	do
	{
		auto recvLen = recv(session_->GetSocket(), session_->GetRecvRef().pBuf + recvPos, PACKET_SIZE, 0);
		if (recvLen == 0)
		{
			/// ���� ����
			CloseProcess(session_);
			return false;
		}

		if (recvLen < 0)
		{
			auto error = ::WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				continue;
			}

			if (error != WSA_IO_PENDING)
			{
				// ����
				return false;
			}
		}

		session_->GetRecvRef().remainLen += recvLen;
	} while (false);

	return true;
}


void CNetworkSelectMode::CloseProcess(CSession* session_)
{
	FD_CLR(session_->GetSocket(), &m_MasterFd);
	CSessionManager::GetInstance()->ReleaseSession(session_);
}



