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
	/*
	if (!FD_ISSET(fd, &read_set))
	{
		return true;
	}
	*/

	auto bodyBuffer = builder_.GetBufferPointer();
	auto bodyLength = builder_.GetSize();

	packets::SHeader header;
	header.id = packetId_;
	header.dataSize = bodyLength;
	header.reserv = 0;
	header.version = 0;

	::memcpy_s(m_sendInfo.pBuf, packets::PACKET_HEADER_SIZE, &header, packets::PACKET_HEADER_SIZE);
	::memcpy_s(m_sendInfo.pBuf + packets::PACKET_HEADER_SIZE, bodyLength, bodyBuffer, bodyLength);

	auto re = send(m_Socket, m_sendInfo.pBuf, packets::PACKET_HEADER_SIZE + bodyLength, 0);
	int a = 0;

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

	return true;
}

bool CSession::Retrieve()
{
	m_IsConnected = false;

	return true;
}