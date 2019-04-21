// TestServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ws2tcpip.h>
#include "tbb/concurrent_queue.h"


//#include <google/protobuf/io/coded_stream.h>
//#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
//#include "../Packet/types.pb.h"
//#include "../Packet/test.pb.h"


#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"


#ifdef _DEBUG
#pragma comment(lib, "tbb_debug.lib")
#pragma comment(lib, "libprotobufd.lib")

#else
#pragma comment(lib, "tbb.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif


#pragma comment(lib, "ws2_32")

using namespace std;
//using namespace google;
using namespace packets;

struct SHead
{
	__int16 code;
	__int16 length;
};


int main()
{
	tbb::concurrent_queue<int> test;
	test.push(1);


	char sendBuf[256];







	//LoginReq ddf;
	{
		flatbuffers::FlatBufferBuilder builder;
		auto id = builder.CreateString("Game");
		auto pass = builder.CreateString("Hello!!");
		auto req = CreateLoginReq(builder, id, pass);
		builder.Finish(req);

		SHead header;
		header.code = 1;
		header.length = builder.GetSize();

		memcpy(sendBuf, &header, sizeof(SHead));

		auto xx = builder.GetBufferPointer();
		auto length = builder.GetSize();

		memcpy(sendBuf + sizeof(SHead), xx, length);


		//--------------------------------------------------------------------

		SHead header2;
		memcpy(&header2, sendBuf, sizeof(SHead));

		LoginReq const* rev = GetLoginReq(sendBuf + sizeof(SHead));

		flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(rev), header2.length);

		//flatbuffers::Verifier verifier(flatbuf, length);
		
		//auto re = VerifyLoginReqBuffer(verifier);
		//TEST_EQ(VerifyLo(verifier), true);

		//verifie.
		//VerifyLoginReqBuffer(rev);


		{
			return 0;
		}
		auto x = rev->id()->str();
		auto y = rev->pass()->str();
	}




	


/*

	packets::LoginReq req;
	req.set_id("Game");
	req.set_pass("hello!!");

	SHead header;
	header.code = packets::LOGIN_REQ;
	header.length = req.ByteSize();

	char sendBuf[256];

	__int32 packetSize = sizeof(SHead) + req.ByteSize();

	protobuf::io::ArrayOutputStream ao(sendBuf, packetSize);
	protobuf::io::CodedOutputStream co(&ao);

	co.WriteRaw(&header, sizeof(SHead));
	req.SerializePartialToCodedStream(&co);

	//--------------------------------------------------------------------------

	
	protobuf::io::ArrayInputStream aihead(sendBuf, packetSize);
	protobuf::io::CodedInputStream ci(&aihead);

	SHead header2;
	ci.ReadRaw(&header2, sizeof(SHead));

	void const* remain = NULL;
	int remainSize = 0;
	int expectedSize = header2.length;

	ci.GetDirectBufferPointer(&remain, &remainSize);
	ci.Skip(expectedSize);
	protobuf::io::ArrayInputStream aibody(remain, expectedSize);

	packets::LoginReq rv;
	bool ret = rv.ParseFromZeroCopyStream(&aibody);
	*/


/*
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);


	int server_sockfd, client_sockfd, sockfd; 
	int state, client_len; 
	int pid; 
	int i, max_client, maxfd; 
	int client[FD_SETSIZE];
	
	FILE *fp; 
	
	struct sockaddr_in clientaddr, serveraddr; 
	struct timeval tv; 
	fd_set readfds, otherfds, allfds;
	
	int current_cli_num; 
	char buf[255]; 
	char line[255]; 
	memset(line, 0x00, 255); 
	state = 0;
	current_cli_num = 3;
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
	{ 
		perror("socket error : "); exit(0); 
	}
	
	memset(&serveraddr, 0x00, sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(5000);
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	if (state == -1) 
	{
		perror("bind error : "); 
		exit(0);
	}
	
	state = listen(server_sockfd, 5);
	if (state == -1) 
	{
		perror("listen error : ");
		exit(0);
	} 
	
	client_sockfd = server_sockfd;
	max_client = -1;
	maxfd = server_sockfd;
	
	::memset(client, -1, FD_SETSIZE);
	
	
	FD_ZERO(&readfds); 
	FD_SET(server_sockfd, &readfds); 

	
	printf("\nTCP Server Waiting ..."); 
	
	fflush(stdout); 
	
	while (1) 
	{ 
		allfds = readfds;
		state = select(maxfd + 1, &allfds, NULL, NULL, NULL);
		if (FD_ISSET(server_sockfd, &allfds))
		{ 
			client_len = sizeof(clientaddr);
			client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
			
			printf("\nconnection from (%s , %d)", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			
			for (i = 0; i < FD_SETSIZE; i++)
			{ 
				if (client[i] < 0) 
				{
					client[i] = client_sockfd; 
					printf("\nclientNUM=%d\nclientFD=%d\n", i + 1, client_sockfd); break; 
				} 
			} 
			printf("accept [%d]\n", client_sockfd); printf("===================================\n");
			if (i == FD_SETSIZE)
			{ 
				perror("too many clients\n");
			}
			
			FD_SET(client_sockfd, &readfds);

			if (client_sockfd > maxfd) 
				maxfd = client_sockfd; 
			
			if (i > max_client) 
				max_client = i;
			
			if (--state <= 0) 
				continue;
		} 
		
		for (i = 0; i <= max_client; i++)
		{ 
			if ((sockfd = client[i]) < 0)
			{ 
				continue; 
			} 
			
			if (FD_ISSET(sockfd, &allfds)) 
			{
				memset(buf, 0x00, 255);


				if (recv(sockfd, buf, 255, 0) <= 0)
				{ 
					closesocket(sockfd);
					perror("Close sockfd : ");
					FD_CLR(sockfd, &readfds); client[i] = -1; 
				}

				printf("[%d]RECV %s\n", sockfd, buf);
				send(sockfd, buf, 255, 0); 
				
				if (--state <= 0) break; 
			} 
		} 
	}


    std::cout << "Hello World!\n"; 

	*/
}
