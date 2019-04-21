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


using namespace packets;

using namespace std;

#pragma comment(lib, "ws2_32")

#define DESERIALIZE(packet, buff) \
	packets::Get##packet(##buff) 

int main()
{

	/*
	{
		flatbuffers::FlatBufferBuilder builder;
		auto id = builder.CreateString("hhhh");
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


		auto rev = DESERIALIZE(LoginReq, sendBuf + packets::PACKET_HEADER_SIZE);




		//auto rev = GetLoginReq(sendBuf + packets::PACKET_HEADER_SIZE);
		auto x = rev->id()->str();
		auto y = rev->pass()->str();
		//auto id = req->id()->c_str();
		//auto pass = req->pass()->c_str();

	}

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


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
