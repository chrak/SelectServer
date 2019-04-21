// TestServerContents.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "ServerManager.h"

#include <stdlib.h>
#include <iostream>

/*
#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ws2tcpip.h>
#include "tbb/concurrent_queue.h"
*/

#include <winsock2.h>



int main()
{
	
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





	CServerManager::Init();

	CNetworkMode::SConfigInfo info;
	info.PortNumber = PORT_NUM;
	info.RecvThreadCount = 4;
	info.SendThreadCount = 2;
	info.BackLogSize = 5;
	info.ServerType = ServerType::CONTENTS_SERVER_TYPE;
	CServerManager::GetInstance()->RegistServer(info);
	CServerManager::GetInstance()->Start();


	std::cout << "아무키나 입력 후 엔터를 치면 종료됩니다.";
	::getchar();


	CServerManager::Release();	


}

