#pragma once
#include "stdafx.h"
#include "ThreadRegister.h"


template <typename T>
CTaskThreadRegister<T>::CTaskThreadRegister()
	: m_IsRunning(false),
	  m_IsClose(false),
 	  m_ThreadCount(0)
{
}

template <typename T>
CTaskThreadRegister<T>::~CTaskThreadRegister()
{
}

template <typename T>
bool CTaskThreadRegister<T>::RegisterThreadFunc(__int32 threadCount_, bool enableMessage_)
{
	m_IsRunning = true;
	m_ThreadCount = threadCount_;

	for (int i = 0; i < threadCount_; ++i)
	{

		SThreadMessage* message = ALLOCATE_NEW(SThreadMessage);
		if (message != NULL)
		{
			m_MessageVec.push_back(message);
		}
		else
		{
			// �α�
			return false;
		}

		SWaitingEvent* waitingEvent = ALLOCATE_NEW(SWaitingEvent);
		waitingEvent->WaitingEvent = ::WSACreateEvent();

		std::thread hendle;
		/// ������ ����
		if (enableMessage_)
		{
			waitingEvent->Message = message;
			waitingEvent->ThreadHandler = std::thread([&]() {
				MessageProc(waitingEvent);
			});
		}
		else
		{
			waitingEvent->ThreadHandler = std::thread([&]() {
				LoopProc(waitingEvent);
			});
		}
		
		/// ������ �����尡 ����� �� ������ ���
		::WaitForSingleObject(waitingEvent->WaitingEvent, INFINITE);
	}

	return true;
}


template <typename T>
unsigned CALLBACK CTaskThreadRegister<T>::MessageProc(SWaitingEvent* waitingInfo_)
{
	if (waitingInfo_ == NULL)
	{
		// �α�
		return 0;
	}

	::WSASetEvent(waitingInfo_->WaitingEvent);

	SThreadMessage* message = waitingInfo_->Message;
	if (message == NULL)
	{
		// �α�
		return 0;
	}

	while (IsRunning())
	{
		::WaitForSingleObject(message->WaitingEvent, INFINITE);

		// ť�� �޽����� ���������� ����
		while (!message->Queue.empty())
		{
			CTaskBase* obj = NULL;
			if (message->Queue.try_pop(obj))
			{
				if (Process(obj))
				{
					SAFE_DELETE(obj);
					break;
				}
			}

			SAFE_DELETE(obj);
		}

		::WSAResetEvent(message->WaitingEvent);
	}

	::WSACloseEvent(waitingInfo_->WaitingEvent);
	/// ������ ���� ���;� ���̴�
	m_WaitEventQueue.push(waitingInfo_);

	return 0;
}


template <typename T>
unsigned CALLBACK CTaskThreadRegister<T>::LoopProc(SWaitingEvent* waitingInfo_)
{
	if (waitingInfo_ == NULL)
	{
		// �α�
		return 0;
	}

	::WSASetEvent(waitingInfo_->WaitingEvent);

	while (IsRunning())
	{	
		if (!Process(NULL))
		{
			break;
		}
	}

	::WSACloseEvent(waitingInfo_->WaitingEvent);
	m_WaitEventQueue.push(waitingInfo_);
	
	return 0;
}


template <typename T>
void CTaskThreadRegister<T>::PushTask(__int32 index_, CTaskBase* task_)
{
	if (task_ == NULL)
	{
		// �α�
		return;
	}

	if (m_MessageVec.size() <= index_ || index_ < 0)
	{
		// �α�
		return;
	}

	SThreadMessage* message = m_MessageVec[index_];
	if (message == NULL)
	{
		// �α�
		return;
	}

	message->Queue.push(task_);
	::WSASetEvent(message->WaitingEvent);
}

/// ��� �����忡 task �߰�
template <typename T>
void CTaskThreadRegister<T>::PushTaskAll(CTaskBase* task_)
{
	if (task_ == NULL)
	{
		// �α�
		return;
	}
}


template <typename T>
void CTaskThreadRegister<T>::FlushMessage()
{
	for (auto message : m_MessageVec)
	{
		::WSASetEvent(message->WaitingEvent);
	}
}


template <typename T>
void CTaskThreadRegister<T>::Close()
{
	FlushMessage();
	m_IsRunning = false;   ///< ���⼭�� ������ ���������Բ��� ����

	while (m_ThreadCount > 0)
	{
		if (!m_WaitEventQueue.empty())
		{
			SWaitingEvent* waitEvent;
			if (m_WaitEventQueue.try_pop(waitEvent))
			{
				::Sleep(100);
				waitEvent->ThreadHandler.join();
				SAFE_DELETE(waitEvent->Message);
				SAFE_DELETE(waitEvent);
				--m_ThreadCount;
			}
		}
		::Sleep(1);
	}

	m_IsClose = true;
}


template <typename T>
bool CTaskThreadRegister<T>::IsRunning() const
{
	return m_IsRunning;
}

template <typename T>
bool CTaskThreadRegister<T>::IsClose() const
{
	return m_IsClose;
}

