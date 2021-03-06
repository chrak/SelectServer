#include "pch.h"
#include "CommonDef.h"
#include "NetworkSelectMode.h"
#include "SessionManager.h"
#include "ServerBase.h"
#include "PacketDef.h"


CNetworkSelectMode::CNetworkSelectMode()
{
	m_TimeOut.tv_sec = SELECT_TIMEOUT_SEC;
	m_TimeOut.tv_usec = 0;
}
CNetworkSelectMode::~CNetworkSelectMode()
{
}

void CNetworkSelectMode::Init(SOCKET const socket_, SConfigInfo const& config_)
{
	FD_ZERO(&m_MasterFd);
	FD_SET(socket_, &m_MasterFd);
}


bool CNetworkSelectMode::TransferProcess(CServerBase* server_)
{
	auto readSet = m_MasterFd;
	auto writeSet = m_MasterFd;
	//auto re = select(0, &readSet, &writeSet, 0, &m_TimeOut);
	auto re = select(0, &readSet, 0, 0, &m_TimeOut);
	if (re == -1)
	{
		// 로그
		return false;
	}
	else if (re == 0)
	{
		// 타임아웃
		return false;
	}

	auto listener = server_->GetListenerRef();
	auto readableCount = re;

	if (FD_ISSET(listener.GetSocket(), &readSet))
	{
		// 쓰레드를 태운다고 가정할 떄 fd_set에 락 거는 방법 말고 괜찮은 방법 생각중
		// auto task = ALLOCATE_NEW(CSelectListenTask);
		// task->SetData(listener.GetSocket());
		// listener.PushTask(task);
		
		LoginProcess(server_);

		--readableCount;
	}

	if (readableCount > 0)
	{
		CSessionManager<CSession>::GetInstance()->ExecuteSessionAll([&](CSessionManager<CSession>::SESSION_MAP const& sessionMap) -> bool{
			for (auto const & pair : sessionMap)
			{
				auto session = pair.second;
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
				// 수신 버퍼가 없다
				continue;
			}

			// 로그
			return false;
		}

		CSession* session = CSessionManager<CSession>::GetInstance()->NewSession(clientSocket);
		if (session)
		{
			SetSockOption(session->GetSocket());
			FD_SET(session->GetSocket(), &m_MasterFd);
		}
		else
		{
			// 로그
		}

	} while (false);

	return true;
}


bool CNetworkSelectMode::RecvProcess(CSession* session_)
{
	if (!session_->IsConnected())
	{
		// 로그
		return false;
	}

	__int32 recvPos = 0;

	/// 남아있는 버퍼가 있었다면 그 버퍼를 가장 앞단에 두고 그 다음부터 받게끔 처리 
	if (session_->GetRecvRef().RemainLen > 0)
	{
		auto recvInfo = session_->GetRecvRef();

		::memcpy_s(&recvInfo.pBuf, recvInfo.RemainLen, &recvInfo.pBuf + recvInfo.PrevRecvPos, recvInfo.RemainLen);
		recvPos += recvInfo.RemainLen;
	}

	do
	{
		auto recvLen = recv(session_->GetSocket(), session_->GetRecvRef().pBuf + recvPos, PACKET_SIZE, 0);
		if (recvLen == 0)
		{
			/// 소켓 종료
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
				// 에러
				CloseProcess(session_);
				return false;
			}
		}

		session_->GetRecvRef().RemainLen += recvLen;
	} while (false);

	return true;
}


void CNetworkSelectMode::CloseProcess(CSession* session_)
{
	FD_CLR(session_->GetSocket(), &m_MasterFd);
	CSessionManager<CSession>::GetInstance()->ReleaseSession(session_);
}

void CNetworkSelectMode::SetSockOption(SOCKET const socket_)
{
	//LINGER ls;
	//ls.l_onoff = 1;
	//ls.l_linger = 0;
	//::setsockopt(m_listenSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char *>(&ls), sizeof(ls));

	bool bValid = true;
	::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&bValid), sizeof(bValid));

	__int32 resvSize = PACKET_SIZE;
	__int32 sendSize = PACKET_SIZE;
	setsockopt(socket_, SOL_SOCKET, SO_RCVBUF, (char*)&resvSize, sizeof(resvSize));
	setsockopt(socket_, SOL_SOCKET, SO_SNDBUF, (char*)&sendSize, sizeof(sendSize));
}



