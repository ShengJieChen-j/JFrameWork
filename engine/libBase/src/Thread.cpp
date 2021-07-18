#include "stdafx.h"
#include "Thread.h"
#include "TypeDefine.h"
#include "ExceptionHandler.h"

#if defined WIN32
#include <mmsystem.h>
#include <process.h>

namespace base
{
	CThread::CThread(IThreadEvent& event)
		: m_event(event)
		, m_hThread(NULL)
		, m_hExitEvent(NULL)
		, m_hWorkEvent(NULL)
	{
		m_id = 0;
		m_dwWorkInterval = 0;
		m_nStatus = STATUS_INIT;
	}

	CThread::~CThread()
	{
		if (STATUS_INIT == m_nStatus)
			return;

		// resume if suspend
		if (STATUS_SUSPEND == m_nStatus)
			this->Resume();

		// close thread...
		if (STATUS_CLOSING != m_nStatus)
			this->Close();

		// waiting for close
		if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, TIME_WAITINGCLOSE))
		{
			::TerminateThread(m_hThread, 4444);
		}

		// release all resources
		if (m_hThread)
		{
			::CloseHandle(m_hThread);
		}

		if (m_hWorkEvent)
		{
			::CloseHandle(m_hWorkEvent);
			m_hWorkEvent = NULL;
		}

		if (m_hExitEvent)
		{
			::CloseHandle(m_hExitEvent);
			m_hExitEvent = NULL;
		}

		m_id = 0;
	}

	bool CThread::Close()
	{
		if (::SetEvent(m_hExitEvent))
		{
			m_nStatus = STATUS_CLOSING;
			return true;
		}

		return false;
	}

	bool CThread::Init(bool bSuspend/*=true*/, DWORD dwWorkInterval/*=INFINITE*/)
	{
		if (STATUS_INIT != m_nStatus)
			return false;

		// exit event
		m_hExitEvent = ::CreateEvent(NULL,	// no security
			FALSE,	// auto reset
			FALSE,	// initial event reset
			NULL);	// no name
		if (!m_hExitEvent)
			return false;

		// work event
		m_hWorkEvent = ::CreateEvent(NULL,	// no security
			FALSE,	// auto reset
			FALSE,	// initial event reset
			NULL);	// no name
		if (!m_hWorkEvent)
			return false;

		// thread
		DWORD dwCreationFlags = 0;
		if (bSuspend)
			dwCreationFlags = CREATE_SUSPENDED;


		typedef unsigned long(__stdcall* THREAD_FUNC)(void*);
		m_hThread = ::CreateThread(NULL,		// default security
			0,							        // default stack size
			(THREAD_FUNC)CThread::RunThread,	// pointer to thread routine
			(void*)this,				        // argument for thread
			dwCreationFlags,			        // start it right away if 0, else suspend
			(unsigned long*)&m_id);

		if (!m_hThread)
		{
			::CloseHandle(m_hExitEvent);
			m_hExitEvent = NULL;

			::CloseHandle(m_hWorkEvent);
			m_hWorkEvent = NULL;

			return false;
		}

		m_dwWorkInterval = dwWorkInterval;

		if (bSuspend)
			m_nStatus = STATUS_SUSPEND;
		else
			m_nStatus = STATUS_RUNNING;

		return true;
	}

	void CThread::Resume(void)
	{
		if (STATUS_SUSPEND != m_nStatus)
			return;

		if (-1 != ::ResumeThread(m_hThread))
			m_nStatus = STATUS_RUNNING;
	}

	void CThread::Suspend(void)
	{
		if (STATUS_RUNNING != m_nStatus)
			return;

		if (-1 != ::SuspendThread(m_hThread))
			m_nStatus = STATUS_SUSPEND;
	}

	CThread* CThread::CreateNew(IThreadEvent& refEvent, bool bSuspend/* = SUSPEND*/, DWORD dwWorkInterval/* = INFINITE*/)
	{
		CThread* pThread = new CThread(refEvent);
		if (!pThread)
			return NULL;

		if (!pThread->Init(bSuspend, dwWorkInterval))
		{
			delete pThread;
			return NULL;
		}

		return pThread;
	}

	DWORD WINAPI CThread::RunThread(LPVOID pThreadParameter)
	{
		CThread* pThread = (CThread*)pThreadParameter;
		if (!pThread)
			return 1;

		base::InitThreadExceptionHandler();

		// init
		if (-1 == pThread->m_event.OnThreadCreate())
			return 2;

		// work now.
		HANDLE hEvent[2] = { pThread->m_hExitEvent, pThread->m_hWorkEvent };
		for (;;)
		{
			DWORD dwTimeWait = pThread->m_dwWorkInterval;
			int rval = ::WaitForMultipleObjects(2, hEvent, false, dwTimeWait);
			if (WAIT_OBJECT_0 == rval)
			{
				// exit event is set
				break;
			}
			else if (WAIT_OBJECT_0 + 1 == rval)
			{
				// work event is set
				if (-1 == pThread->m_event.OnThreadEvent())
					break;
			}
			else if (WAIT_TIMEOUT == rval)
			{
				// work interval is time out
				if (-1 == pThread->m_event.OnThreadProcess())
					break;

			}
			else
			{
				// here should log error msg
				break;
			}
		}

		pThread->m_nStatus = CThread::STATUS_CLOSED;
		return pThread->m_event.OnThreadDestroy();
	}

}


#elif defined __LINUX__
#include <pthread.h>
#include <sys/time.h>
base::CThread::CThread(IThreadEvent& event)
	: m_event(event)
	, m_hThread(0)
	, m_id(0)
	, m_dwWorkInterval(0)
	, m_nStatus(STATUS_INIT)
	, m_bWorkEvent(false)
{
}

base::CThread::~CThread()
{
	if (STATUS_INIT == m_nStatus)
		return;

	if (STATUS_CLOSING != m_nStatus)
		Close();

	// waiting for close
	void* pReturn = 0;
	pthread_join(m_hThread, &pReturn);

	m_id = 0;
}

bool base::CThread::Close()
{
	m_nStatus = STATUS_CLOSING;
	return true;
}

bool base::CThread::Init(uint32_t dwWorkInterval/*=INFINITE*/)
{
	if (STATUS_INIT != m_nStatus)
		return false;

	if (0 != pthread_create(&m_hThread, NULL, RunThread, (void*)this))
		return false;

	m_dwWorkInterval = dwWorkInterval;
	m_id = (int32_t)m_hThread;
	m_nStatus = STATUS_RUNNING;
	return true;
}

base::CThread* base::CThread::CreateNew(IThreadEvent& refEvent, bool bSuspend/* = SUSPEND*/, int32_t dwWorkInterval/* = INFINITE*/)
{
	CThread* pThread = new CThread(refEvent);
	if (!pThread)
		return NULL;

	if (!pThread->Init(dwWorkInterval))
	{
		delete pThread;
		return NULL;
	}

	return pThread;
}

void* base::CThread::RunThread(void* param)
{
	CThread* pThis = (CThread*)param;
	if (!pThis)
		return (void*)1;

	if (-1 == pThis->m_event.OnThreadCreate())
	{
		return (void*)-2;
	}
	timeval tmlast = { 0 };
	gettimeofday(&tmlast, NULL);
	while (pThis->m_nStatus != STATUS_CLOSING)
	{
		if (pThis->m_bWorkEvent)
		{
			pThis->m_bWorkEvent = false;
			gettimeofday(&tmlast, NULL);
			if (-1 == pThis->m_event.OnThreadEvent())
				break;
		}
		timeval tmnow = { 0 };
		gettimeofday(&tmnow, NULL);
		if ((tmnow.tv_sec * 1000 + tmnow.tv_usec / 1000) - (tmlast.tv_sec * 1000 + tmlast.tv_usec / 1000) > (long)pThis->m_dwWorkInterval)
		{
			if (-1 == pThis->m_event.OnThreadProcess())
			{
				break;
			}
			tmlast = tmnow;
		}
		usleep(1000);
	}
	pThis->m_nStatus = STATUS_CLOSED;
	return (void*)pThis->m_event.OnThreadDestroy();
}

void base::CThread::SetEvent()
{
	m_bWorkEvent = true;
}

#endif



