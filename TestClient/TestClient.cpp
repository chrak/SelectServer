// TestClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUF_SIZE 1024

#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"
#include "structure.h"
#include "PacketDef.h"
#include "CommonDef.h"



#include "SessionManager.h"
#include "Connector.h"


using namespace packets;

using namespace std;

#pragma comment(lib, "ws2_32")

#define DESERIALIZE(packet, buff) \
	packets::Get##packet(##buff) 

int main()
{

	/*
	CSessionManager<CConnector>::GetInstance()->Init();

	CConnector 




	CSessionManager<CConnector>::GetInstance()->Release();
	*/



	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);

	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_addr;

	string ipAddress = "127.0.0.1";
	short  port = 5000;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		perror("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serv_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		perror("connect() error!");
	}
	else
	{
		puts("Connected..");
	}
		
	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		flatbuffers::FlatBufferBuilder builder;
		auto id = builder.CreateString(message);
		auto pass = builder.CreateString("Hello!!");
		auto req = CreateLoginReq(builder, id, pass);
		builder.Finish(req);


		auto buffer = builder.GetBufferPointer();
		auto length = builder.GetSize();

		packets::SHeader header;
		header.id = packetdef::LoginReq;
		header.dataSize = length;

		char sendBuf[256];

		memcpy(sendBuf, &header, packets::PACKET_HEADER_SIZE);
		memcpy(sendBuf + packets::PACKET_HEADER_SIZE, buffer, length);
		send(sock, sendBuf, packets::PACKET_HEADER_SIZE + length, 0);



		str_len = recv(sock, message, BUF_SIZE - 1, 0);




		message[str_len] = 0;
		//printf("Message from server : %s", message);
	}

	closesocket(sock);
	return 0;
}
