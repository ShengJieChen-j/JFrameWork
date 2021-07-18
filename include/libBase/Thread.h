/******************************************************************
	创建日期：  2021年7月18日 
	文件名称：  Thread.h
	说    明：

	当前版本：  1.00
	作    者：	shengjiechen
	概    述：  支持win,linux平台。

*******************************************************************/
#pragma once
#include <stdint.h>

namespace base
{
	class IThreadEvent
	{
	public:
		virtual			    ~IThreadEvent() {}
		virtual int32_t		OnThreadCreate(void) = 0;
		virtual int32_t		OnThreadDestroy(void) = 0;

		virtual int32_t		OnThreadEvent(void) = 0;
		virtual int32_t		OnThreadProcess(void) = 0;
	};
};

#if defined WIN32

#include <windows.h>
#include "SyncObjs.h"

namespace base
{
	CONST int32_t   TIME_WAITINGCLOSE = 3000;		// 3000ms

	class CThread
	{
	public:
		CThread(IThreadEvent& event);
		virtual ~CThread();
		CThread& operator=(const CThread&) {}
	public:
		enum { SUSPEND = true, RUN = false };
		bool	Init(bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);
		bool	Close(void);
		void	Resume(void);
		void	Suspend(void);

		void	SetEvent(void) { ::SetEvent(m_hWorkEvent); }

		enum { STATUS_INIT = 0, STATUS_SUSPEND, STATUS_RUNNING, STATUS_CLOSING, STATUS_CLOSED };
		int		GetStatus(void) const { return m_nStatus; }
		DWORD   GetThreadID(void) const { return m_id; }
		void    SetThreadAMask(int curAff) { SetThreadAffinityMask(m_hThread, curAff); };

	protected:
		DWORD				m_dwWorkInterval;
		IThreadEvent& m_event;

	private:
		HANDLE				m_hThread;
		HANDLE				m_hExitEvent;
		HANDLE				m_hWorkEvent;

		DWORD				m_id;
		volatile int		m_nStatus;

		static DWORD WINAPI RunThread(LPVOID pThreadParameter);

	public:
		static CThread* CreateNew(IThreadEvent& refEvent, bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);

	};

}; // endof namespace base


#elif defined __LINUX__
#include "types.h"
#include <pthread.h>
namespace base
{
	const uint32_t TIME_WAITINGCLOSE = 3000;		// 3000ms

	class CThread
	{
	public:
		CThread(IThreadEvent& event);
		virtual ~CThread();
	private:
		CThread& operator=(const CThread&) { return *this; }
	public:
		enum { SUSPEND = true, RUN = false };
		bool	Init(uint32_t dwWorkInterval = 0xFFFFFFFF);
		bool	Close(void);
		void	SetEvent(void);

		enum { STATUS_INIT = 0, STATUS_SUSPEND, STATUS_RUNNING, STATUS_CLOSING, STATUS_CLOSED };

		int			GetStatus(void) const { return m_nStatus; }
		uint32_t	GetThreadID(void) const { return m_id; }
		void		SetThreadAMask(int curAff) {}
		pthread_t	GetHandle() { return m_hThread; }
	protected:
		IThreadEvent& m_event;
		pthread_t			m_hThread;
		uint32_t			m_id;
		uint32_t			m_dwWorkInterval;
		volatile int		m_nStatus;
		bool				m_bWorkEvent;

		static void* RunThread(void* pThreadParameter);

	public:
		static CThread* CreateNew(IThreadEvent& refEvent, bool bSuspend = SUSPEND, uint32_t dwWorkInterval = 0xFFFFFFFF);
	};

}; // endof namespace base

#endif

