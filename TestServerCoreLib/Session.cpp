#pragma once
#include "pch.h"
#include "structure.h"
#include "Session.h"
#include "CommonDef.h"

CSession::CSession()
	: m_IsConnected(false),
	m_Index(INVALID_SERIAL_ID)
{
	/// recv의 경우 하나의 버퍼에서 남은 버퍼까지 처리할 예정이므로 최대 크기의 2배로 잡음
	m_sendInfo.pBuf = ALLOCATE_MALLOC(char, PACKET_REMAIN_SIZE);
	m_recvInfo.pBuf = ALLOCATE_MALLOC(char, PACKET_SIZE);  
}

CSession::~CSession()
{
	SAFE_DELETE(m_sendInfo.pBuf);
	SAFE_DELETE(m_recvInfo.pBuf);
}


bool CSession::SendPacket(packetdef::E_PACKET_ID packetId_, flatbuffers::FlatBufferBuilder& builder_)
{
	if (!IsConnected())
	{
		// 로그
		return false;
	}

	auto bodyBuffer = builder_.GetBufferPointer();
	auto bodyLength = builder_.GetSize();
	auto packetLength = packets::PACKET_HEADER_SIZE + bodyLength;

	if (m_sendInfo.RemainLen + packetLength > PACKET_REMAIN_SIZE)
	{
		// 로그
		return false;
	}

	packets::SHeader header;
	header.id = packetId_;
	header.dataSize = bodyLength;
	header.reserv = 0;
	header.version = 0;

	::memcpy_s(m_sendInfo.pBuf + m_sendInfo.RemainLen, packets::PACKET_HEADER_SIZE, &header, packets::PACKET_HEADER_SIZE);
	::memcpy_s(m_sendInfo.pBuf + m_sendInfo.RemainLen + packets::PACKET_HEADER_SIZE, bodyLength, bodyBuffer, bodyLength);

	m_sendInfo.RemainLen += packetLength;

	/// send thread 만드려면 복사도 해야되고 락 또 걸어야
	do
	{
		auto result = send(m_Socket, m_sendInfo.pBuf, packets::PACKET_HEADER_SIZE + bodyLength, 0);
		if (result <= 0)
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				continue;
			}

			// 로그
			return false;
		}

		if (result > 0)
		{
			m_sendInfo.RemainLen -= result;
		}

	} while (false);
	
	return true;
}


bool CSession::Acquire(void* param_)
{
	if (param_ == NULL)
	{
		// 로그
		return false;
	}

	SInfo* info = static_cast<CSession::SInfo*>(param_);
	m_Socket = info->Socket;
	m_Index = info->Index;
	m_IsConnected = true;  ///< 중간에 실패해도 Retrieve에 들어가게 한다면 상관없다

	return true;
}

bool CSession::Retrieve()
{
	m_IsConnected = false;

	return true;
}