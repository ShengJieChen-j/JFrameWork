/******************************************************************
	创建日期：  2021年7月24日
	文件名称：  SyncObjs.h
	说    明：

	当前版本：  1.00
	作    者：	shengjiechen
	概    述：  支持win,linux平台。

*******************************************************************/
#pragma once

#if defined WIN32
#include <windows.h>
#include <tchar.h>
#elif defined __LINUX__
#include <pthread.h>

#if !defined __CYGWIN__
#include <pthread.h>
#endif
#endif

#include "stdio.h"

namespace base
{
	class ILockObj
	{
	public:
		virtual      ~ILockObj() {}
		virtual void Lock(void) = 0;
		virtual void Unlock(void) = 0;
	};

	class CCriticalSection : public ILockObj
	{
	public:
		CCriticalSection()
		{
#if defined WIN32
			InitializeCriticalSection(&m_CritSec);
#elif defined __LINUX__
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
			pthread_mutex_init(&mutex_lock, &attr);
#endif
		};

		virtual ~CCriticalSection()
		{
#if defined WIN32
			DeleteCriticalSection(&m_CritSec);
#elif defined __LINUX__
			pthread_mutex_destroy(&mutex_lock);
#endif

		};

		void Lock(void)
		{
#if defined WIN32
			EnterCriticalSection(&m_CritSec);
#elif defined __LINUX__
			pthread_mutex_lock(&mutex_lock);
#endif
		};

		void Unlock(void)
		{
#if defined WIN32
			LeaveCriticalSection(&m_CritSec);
#elif defined __LINUX__
			pthread_mutex_unlock(&mutex_lock);
#endif
		};

	private:
#if defined WIN32
		CRITICAL_SECTION    m_CritSec;
#elif defined __LINUX__
		pthread_mutexattr_t attr;
		pthread_mutex_t mutex_lock;
#endif
	};

	class CSingleLock
	{
	public:
		CSingleLock(ILockObj* pLock)
			: m_pLock(pLock)
		{
			if (m_pLock)
				m_pLock->Lock();
		}

		~CSingleLock(void)
		{
			if (m_pLock)
			{
				m_pLock->Unlock();
			}
		}

	private:
		ILockObj* m_pLock;
	};

}; 	// endof namespace base 


