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
#include "flatbuffers/flatbuffers.h"
#include "../Packet/LoginReq_generated.h"
#include "structure.h"
#include "PacketDef.h"
#include "CommonDef.h"


#include "DummyClient.h"
#include "DummyCommandTask.h"
#include "SessionManager.h"
#include "Connector.h"



using namespace packets;

using namespace std;

#pragma comment(lib, "ws2_32")

#define DESERIALIZE(packet, buff) \
	packets::Get##packet(##buff) 

int main()
{
	CDummyClient::Init();

	CDummyClient::SInfo info;
	info.ConnectionAddr = "127.0.0.1";
	info.dummyCount = 5;
	info.PortNumber = 5000;
	CDummyClient::GetInstance()->InitInfo(info);
	CDummyClient::GetInstance()->Start();

	while (CDummyClient::GetInstance()->IsRunning())
	{
		std::cout << "명령어를 입력하시오.";
		getchar();

		CDummyCommandTask* task = ALLOCATE_NEW(CDummyCommandTask);
		task->CommandType = CDummyCommandTask::CHAT_TYPE;
		task->CommondMessage = "테스트";
		CDummyClient::GetInstance()->PushTask(0, task);
	};


	CDummyClient::Release();

	return 0;
}
