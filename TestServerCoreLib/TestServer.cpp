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

}
