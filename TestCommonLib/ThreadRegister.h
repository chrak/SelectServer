#pragma once
#include "stdafx.h"
#include <windows.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <thread>
#include "TaskBase.h"
#include "tbb/concurrent_queue.h"


struct SThreadMessage
{
	typedef tbb::concurrent_queue<CTaskBase*> THREAD_QUEUE;
	THREAD_QUEUE Queue;                                ///< Task Thread용 taskQueue
	HANDLE       WaitingEvent;                         ///< taskThread의 다음 작업을 기다리기 위한 핸들

	SThreadMessage()
	{
		WaitingEvent = ::WSACreateEvent();
	}
};


///////////////////////////////////////////////////////////
// 태스크 쓰레드 등록
// <Derived class>
///////////////////////////////////////////////////////////
template <typename T>
class CTaskThreadRegister
{
protected:
	struct SWaitingEvent
	{
		HANDLE WaitingEvent;				  ///< thread의 초기화가 완료될 때까지 기다리기 위한 핸들
		std::thread ThreadHandler;			      ///< thread의 핸들
		SThreadMessage* Message;              ///< task 메시지 큐

		SWaitingEvent() :
			Message(NULL)
		{
		}
	};

	typedef std::vector<SThreadMessage*> THREAD_MESSAGE_VEC;
	typedef tbb::concurrent_queue<SWaitingEvent*> WAIT_EVENT_QUEUE;


protected:
	volatile bool m_IsRunning;		         ///< 쓰레드의 동작 유무
	volatile bool m_IsClose;                 ///< 쓰레드가 끝났는지의 유무 (끝나지도 않았는데 server객체가 delete 되는거 방지)
	__int32       m_ThreadCount;             ///< 쓰레드 개수
	THREAD_MESSAGE_VEC m_MessageVec;         ///< 각 쓰레드에 대한 메세지 백터
	WAIT_EVENT_QUEUE   m_WaitEventQueue;     ///< 대기 이벤트 쓰레드 종료시에 처리하기 위한 큐

public:
	CTaskThreadRegister();
	virtual ~CTaskThreadRegister();

private:

public:
	/// 쓰레드 등록 함수(task 큐, 등록 함수)
	virtual bool RegisterThreadFunc(__int32 threadCount_, bool enableMessage_);

	/// task 추가
	void PushTask(__int32 index_, CTaskBase* task_);

	/// 모든 쓰레드에 task 추가
	void PushTaskAll(CTaskBase* task_);

	/// 쓰레드 개수
	inline __int32 GetThreadCount() const { return m_ThreadCount; } 

	/// task에 대한 flush처리
	virtual void FlushMessage();

	/// 쓰레드 종료
	virtual void Close();

	/// 등록된 쓰레드의 동작 유무
	virtual bool IsRunning() const;


	virtual bool IsClose() const;

	/// 쓰레드 처리
	virtual bool Process(CTaskBase*) = 0;


private:
	unsigned CALLBACK MessageProc(SWaitingEvent* waitingInfo_);
	unsigned CALLBACK LoopProc(SWaitingEvent* waitingInfo_);
};

