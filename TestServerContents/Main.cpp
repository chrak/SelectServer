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
	CServerManager::Init();

	CNetworkMode::SConfigInfo info;
	info.PortNumber = PORT_NUM;
	info.RecvThreadCount = 6;   // worker겸용
	info.BackLogSize = 5;
	info.ServerType = ServerType::CONTENTS_SERVER_TYPE;
	CServerManager::GetInstance()->RegistServer(info);
	CServerManager::GetInstance()->Start();

	std::cout << "아무키나 입력 후 엔터를 치면 종료됩니다.";
	::getchar();


	CServerManager::Release();	


}

