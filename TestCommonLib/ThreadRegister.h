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
	THREAD_QUEUE Queue;                                ///< Task Thread�� taskQueue
	HANDLE       WaitingEvent;                         ///< taskThread�� ���� �۾��� ��ٸ��� ���� �ڵ�

	SThreadMessage()
	{
		WaitingEvent = ::WSACreateEvent();
	}
};


///////////////////////////////////////////////////////////
// �½�ũ ������ ���
// <Derived class>
///////////////////////////////////////////////////////////
template <typename T>
class CTaskThreadRegister
{
protected:
	struct SWaitingEvent
	{
		HANDLE WaitingEvent;				  ///< thread�� �ʱ�ȭ�� �Ϸ�� ������ ��ٸ��� ���� �ڵ�
		std::thread ThreadHandler;			      ///< thread�� �ڵ�
		SThreadMessage* Message;              ///< task �޽��� ť

		SWaitingEvent() :
			Message(NULL)
		{
		}
	};

	typedef std::vector<SThreadMessage*> THREAD_MESSAGE_VEC;
	typedef tbb::concurrent_queue<SWaitingEvent*> WAIT_EVENT_QUEUE;


protected:
	volatile bool m_IsRunning;		         ///< �������� ���� ����
	volatile bool m_IsClose;                 ///< �����尡 ���������� ���� (�������� �ʾҴµ� server��ü�� delete �Ǵ°� ����)
	__int32       m_ThreadCount;             ///< ������ ����
	THREAD_MESSAGE_VEC m_MessageVec;         ///< �� �����忡 ���� �޼��� ����
	WAIT_EVENT_QUEUE   m_WaitEventQueue;     ///< ��� �̺�Ʈ ������ ����ÿ� ó���ϱ� ���� ť

public:
	CTaskThreadRegister();
	virtual ~CTaskThreadRegister();

private:

public:
	/// ������ ��� �Լ�(task ť, ��� �Լ�)
	virtual bool RegisterThreadFunc(__int32 threadCount_, bool enableMessage_);

	/// task �߰�
	void PushTask(__int32 index_, CTaskBase* task_);

	/// ��� �����忡 task �߰�
	void PushTaskAll(CTaskBase* task_);

	/// ������ ����
	inline __int32 GetThreadCount() const { return m_ThreadCount; } 

	/// task�� ���� flushó��
	virtual void FlushMessage();

	/// ������ ����
	virtual void Close();

	/// ��ϵ� �������� ���� ����
	virtual bool IsRunning() const;


	virtual bool IsClose() const;

	/// ������ ó��
	virtual bool Process(CTaskBase*) = 0;


private:
	unsigned CALLBACK MessageProc(SWaitingEvent* waitingInfo_);
	unsigned CALLBACK LoopProc(SWaitingEvent* waitingInfo_);
};

